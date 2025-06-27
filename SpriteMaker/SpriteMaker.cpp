#include <iostream>
#include <string>
#include <filesystem>
#include "Image.h"
namespace fs = std::filesystem;



int main(int argc, char* argv[])
{
    /*char path[1024];
    path[0] = 0;
    if (_getcwd(path, sizeof(path)) != nullptr) 
        strcat_s(path, "\\");*/
    //strcat_s(path, argv[0]);
    /*std::cout << argv[0];*/

//    std::cout << "PLEASE WAIT...\n";
//    std::string file_path = "./input_images/snake.bmp";
//    std::vector<Image> images;
    std::string input_images_path = "./input_images";
    for (const auto& entry : fs::directory_iterator(input_images_path))
    {
        std::string entro = entry.path().string();
        std::cout << entro << std::endl;
    }
 //   Image ex1(file_path);
//    std::cout << "DONE!\n";
}

