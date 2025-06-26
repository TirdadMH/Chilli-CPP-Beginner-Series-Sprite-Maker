#include <iostream>
#include "Image.h"

int main()
{
	std::string path = "./images/bmp/snake.bmp";
	Image snake_image(path);
	snake_image.fill_region(50, 20, 100, 200, 0, 0, 255, 255);
}

