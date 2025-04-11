#include "Image.h" 
#include <string>
#include <iostream>

void save_png(const unsigned char* bytes, int width, int height)
{
    std::string file_path = "Resources/Drawings/copy_image.png";  
       
    // * 4 for RGBA channel
    if (stbi_write_png(file_path.c_str(), width, height, 4, bytes, width * 4))
    {
        std::cout << "Image saved successfully to: " << file_path << std::endl;
    }
    else 
    {
        std::cerr << "Failed to save image!" << std::endl;
    }
}