#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include "Image.h"
namespace fs = std::filesystem;

std::vector<std::string> scan_files(std::vector<std::string> images)
{
    std::string input_images_path = "./input_images";
    for (const auto& entry : fs::directory_iterator(input_images_path))
    {
        std::string image_path = entry.path().string();
        for (int i = 0; i < image_path.length(); i++)
            if (image_path[i] == '\\')
                image_path[i] = '/';
        images.push_back(image_path);
    }
    return images;
}

int find_number_of_output_files()
{
    int number_of_files = 0;
    std::string output_files_path = "./output_files";
    for (const auto& entry : fs::directory_iterator(output_files_path)) number_of_files++;
    return number_of_files;
}