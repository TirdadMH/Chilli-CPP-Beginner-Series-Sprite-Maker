#pragma once

struct BMPFileHeader
{
	uint16_t file_type{ 0x4D42 };		// File type always BM which is 0x4D42
	uint32_t file_size{ 0 };			// Size of the file (in bytes)
	uint16_t reserved1{ 0 };
	uint16_t reserved2{ 0 };
	uint32_t offset_data{ 0 };
};

struct BMPInfoHeader
{
	uint32_t size{ 0 };
	int32_t width{ 0 };					// width of bitmap in pixels
	int32_t height{ 0 };				// height of bitmap in pixels
	uint16_t planes{ 1 };
	uint16_t bit_count{ 0 };			// No. of bits per pixel
	uint32_t compression{ 0 };			// 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	uint32_t size_image{ 0 };
	int32_t x_pixels_per_meter{ 0 };
	int32_t y_pixels_per_meter{ 0 };
	uint32_t colors_used{ 0 };
	uint32_t colors_important{ 0 };
};

struct BMPColorHeader 
{
	uint32_t red_mask{ 0x00ff0000 };	// Bit mask for the red channel
	uint32_t green_mask{ 0x0000ff00 };	// Bit mask for the green channel
	uint32_t blue_mask{ 0x000000ff };	// Bit mask for the blue channel
	uint32_t alpha_mask{ 0xff000000 };	// Bit mask for the alpha channel
	uint32_t color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
	uint32_t unused[16]{ 0 };
};


class Image
{
public:
	void Load(const char* path);

private:
	int fileSize;
	int width;
	int height;
};

