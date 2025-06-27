#include "Image.h"
#include <iostream>
#include <fstream>



Image::Image(const char* file_path)
{
	read(file_path);


}

void Image::read(const char* file_path)
{
	unsigned char* data = stbi_load(file_path, &width, &height, &image_channels, desired_channels);

	if (!data) 
		throw std::runtime_error("Failed to load image. The file may be missing, corrupted, or in an unsupported format.");

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
