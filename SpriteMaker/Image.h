#pragma once
#include <array>
#include <vector>
#include <string>

class Image
{
public:
	Image(std::string file_path);

private:
	void read(std::string file_path);
	void get_file_name(std::string file_path);
	void export_file();

private:
	bool is_valid = true;
	int width;
	int height;
	int image_channels;
	int desired_channels = 3; // 3 for RGB, 4 for RGBA
	std::vector<std::array<int, 5>> pixels;
	std::string file_name;
};

