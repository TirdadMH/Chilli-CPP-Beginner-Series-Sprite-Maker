#include "Image.h"
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Image::Image(std::string file_path)
{
	read(file_path);
	get_file_name(file_path);
	export_file();
}

void Image::read(std::string file_path)
{
	unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &image_channels, desired_channels);

	if (!data)
	{
		is_valid = false;
		return;
	}

	if (width <= 0 || height <= 0) 
	{
		stbi_image_free(data);
		throw std::runtime_error("Loaded image has invalid dimensions.");
	}

	if (image_channels != 3 && image_channels != 4) 
	{
		stbi_image_free(data);
		throw std::runtime_error("Unsupported number of channels in input image. Must be RGB or RGBA.");
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = (y * width + x) * desired_channels;
			unsigned char r = data[index + 0];
			unsigned char g = data[index + 1];
			unsigned char b = data[index + 2];
			std::array<int, 5> pixel_data{ x, y, (int)r, (int)g, (int)b };
			pixels.push_back(pixel_data);
		}
	}
}

void Image::get_file_name(std::string file_path)
{
	if (is_valid)
	{
		int last_slash = 0;
		for (int i = 0; i < file_path.length(); i++)
			if (file_path[i] == '/')
				last_slash = i;
		int file_name_length = (file_path.length() - 1) - last_slash;
		file_name = file_path.substr(last_slash + 1, file_name_length);
	}
}

void Image::export_file()
{
	if (is_valid)
	{
		std::string out_file_name = "./output_files/" + file_name + ".txt";
		std::ofstream ofile(out_file_name);
		for (auto pixel : pixels)
			ofile << "gfx.PutPixel(" << pixel[0] << " + x, " << pixel[1] << " + y, " << pixel[2] << ", " << pixel[3] << ", " << pixel[4] << ");" << std::endl;
		ofile.close();
		std::cout << "Saved " << file_name + ".txt" << " to ./output_files.\n";
	}
}