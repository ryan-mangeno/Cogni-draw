#include "Image.h" 
#include <string>
#include <iostream>
#include <filesystem>




void save_png(const unsigned char* bytes, int width, int height)
{   
    const std::string file_path = "Resources/Drawings/drawing.png";

    unsigned char* resized = new unsigned char[512 * 512 * 4];
    // resizing to 512x512 always to save token usage with api
    if(!stbir_resize_uint8(bytes, width, height, 0, resized, 512, 512, 0, 4))
        std::cout << "Error resizing\n";
	
    else
    {
        // * 4 for RGBA channel
        if (stbi_write_png(file_path.c_str(), 512, 512, 4, resized, 512 * 4))
        {
        }
        else
        {
            std::cerr << "Failed to save image!" << std::endl;
        }

        delete[] resized;
    }

}