#include "Image.h"
#include <iostream>
#include <fstream>



Image::Image(std::string file_name)
{
	read(file_name);
}

Image::Image(int32_t width, int32_t height, bool has_alpha)
{
	if (width <= 0 || height <= 0)
		throw std::runtime_error("The image width and height must be positive numbers.");
	bmp_info_header.width = width;
	bmp_info_header.height = height;
	if (has_alpha)
	{
		bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
		file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
		bmp_info_header.bit_count = 32;
		bmp_info_header.compression = 3;
		row_stride = width * 4;
		data.resize(row_stride * height);
		file_header.file_size = file_header.offset_data + data.size();
	}
	else
	{
		bmp_info_header.size = sizeof(BMPInfoHeader);
		file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
		bmp_info_header.bit_count = 24;
		bmp_info_header.compression = 0;
		row_stride = width * 3;
		data.resize(row_stride * height);
		uint32_t new_stride = make_stride_aligned(4);
		file_header.file_size = file_header.offset_data + data.size() + bmp_info_header.height * (new_stride - row_stride);
	}
}

void Image::write(std::string file_name)
{
	std::ofstream of{ file_name, std::ios_base::binary };
	if (of)
	{
		if (bmp_info_header.bit_count == 32)
			write_headers_and_data(of);
		else if (bmp_info_header.bit_count == 24)
		{
			if (bmp_info_header.width % 4 == 0)
				write_headers_and_data(of);
			else
			{
				uint32_t new_stride = make_stride_aligned(4);
				std::vector<uint8_t> padding_row(new_stride - row_stride);
				write_headers(of);
				for (int y = 0; y < bmp_info_header.height; y++)
				{
					of.write((const char*)(data.data() + row_stride * y), row_stride);
					of.write((const char*)padding_row.data(), padding_row.size());
				}
			}

		}
		else
			throw std::runtime_error("Unable to open the output image file.");
	}
}

void Image::fill_region(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t B, uint8_t G, uint8_t R, uint8_t A)
{
	if (x0 + w > (uint32_t)bmp_info_header.width || y0 + h > (uint32_t)bmp_info_header.height) 
	{
		throw std::runtime_error("The region does not fit in the image!");
	}

	uint32_t channels = bmp_info_header.bit_count / 8;
	for (uint32_t y = y0; y < y0 + h; ++y)
	{
		for (uint32_t x = x0; x < x0 + w; ++x)
		{
			data[channels * (y * bmp_info_header.width + x) + 0] = B;
			data[channels * (y * bmp_info_header.width + x) + 1] = G;
			data[channels * (y * bmp_info_header.width + x) + 2] = R;
			if (channels == 4)
			{
				data[channels * (y * bmp_info_header.width + x) + 3] = A;
			}
		}
	}
}

void Image::read(std::string file_name)
{
	std::ifstream inp{ file_name, std::ios_base::binary };
	if (inp)
	{
		inp.read((char*)&file_header, sizeof(file_header));
		if (file_header.file_type != 0x4D42)
			throw std::runtime_error("Error! Unrecognized file format.");
		inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));
		if (bmp_info_header.bit_count == 32)
		{
			if (bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader)))
			{
				inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
				check_color_header(bmp_color_header);
			}
			else
			{
				std::cerr << "Warning! the file \"" << file_name << "\" does not seem to contain bit mask information\n";
				throw std::runtime_error("Error! Unrecognized file format.");
			}
		}
		inp.seekg(file_header.offset_data, inp.beg);

		if (bmp_info_header.bit_count == 32)
		{
			bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
		}
		else
		{
			bmp_info_header.size = sizeof(BMPInfoHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
		}
		file_header.file_size = file_header.offset_data;
		if (bmp_info_header.height < 0)
			throw std::runtime_error("The Program can treat only BMP Images with the origin in the buttom left corner");

		data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

		if (bmp_info_header.width % 4 == 0)
		{
			inp.read((char*)data.data(), data.size());
			file_header.file_size += data.size();
		}
		else
		{
			row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
			uint32_t new_stride = make_stride_aligned(4);
			std::vector<uint8_t> padding_row(new_stride - row_stride);
			for (int y = 0; y < bmp_info_header.height; y++)
			{
				inp.read((char*)(data.data() + row_stride * y), row_stride);
				inp.read((char*)padding_row.data(), padding_row.size());
			}
			file_header.file_size += data.size() + bmp_info_header.height * padding_row.size();
		}

	}
	else
		throw std::runtime_error("Unable to open the input image file.");
}

void Image::check_color_header(BMPColorHeader& bmp_color_header)
{
	BMPColorHeader expected_color_header;
	if (expected_color_header.red_mask != bmp_color_header.red_mask ||
		expected_color_header.green_mask != bmp_color_header.green_mask ||
		expected_color_header.blue_mask != bmp_color_header.blue_mask ||
		expected_color_header.alpha_mask != bmp_color_header.alpha_mask)
		throw std::runtime_error("Unexpected color mask format! The Program expects the pixel data to be in the BGRA format");
	if (expected_color_header.color_space_type != bmp_color_header.color_space_type)
		throw std::runtime_error("Unexpected color space type! the program expects sRGB value");
}

void Image::write_headers(std::ofstream& of)
{
	of.write((const char*)&file_header, sizeof(file_header));
	of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
	if (bmp_info_header.bit_count == 32)
		of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
}

void Image::write_headers_and_data(std::ofstream& of)
{
	write_headers(of);
	of.write((const char*)data.data(), data.size());
}

uint32_t Image::make_stride_aligned(uint32_t align_stride)
{
	uint32_t new_stride = row_stride;
	while (new_stride % align_stride != 0)
		new_stride++;
	return new_stride;
}
