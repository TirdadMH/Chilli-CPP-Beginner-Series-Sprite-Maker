#include "utils.h"

int main(int argc, char* argv[])
{
    /*
    char path[1024];
    path[0] = 0;
    if (_getcwd(path, sizeof(path)) != nullptr) 
        strcat_s(path, "\\");
    strcat_s(path, argv[0]);
    std::cout << argv[0];
    */
    std::cout << "Starting...\n----------\n";
    std::vector<std::string> images_path = scan_files(images_path);
    std::cout << "Found " << images_path.size() << " files.\n";
    for (const auto& path : images_path) std::cout << path << std::endl;
    std::cout << "----------\n" << "Processing...\n";
    int index = 0;
    for (const auto& path : images_path)
    {
        std::cout << "(" << index + 1 << "/" << images_path.size() <<") ";
        Image image(path);
        index++;
    }
    std::cout << "Operation done for " << find_number_of_output_files() << " files.";
    std::cout << "\n==============================================================\n";
    system("pause");
    return 0;
}

