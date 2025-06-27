#pragma once
#include <array>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


class Image
{
public:
	Image(const char* file_path);

private:
	void read(const char* file_path);

private:
	int width;
	int height;
	int image_channels;
	int desired_channels = 3; // 3 for RGB, 4 for RGBA
	std::vector<std::array<int, 5>> pixels;
};

