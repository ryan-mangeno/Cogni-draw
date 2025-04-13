#include "Image.h" 
#include <string>
#include <iostream>
#include <filesystem>


static std::string get_unique_filepath(const std::string& original_path) {
    std::filesystem::path path(original_path);

    if (!std::filesystem::exists(path)) 
    {
        return original_path;  // safe to use as is
    }

    std::filesystem::path parent = path.parent_path();
    std::filesystem::path stem = path.stem();         // filename without extension
    std::filesystem::path extension = path.extension();

    uint32_t counter = 1;
    std::filesystem::path new_path;


    // we know it exists if we are here, we can use a do while
    do
    {
        std::ostringstream ss;
        ss << stem.string() << "_" << counter << extension.string();
        new_path = parent / ss.str();
        counter++;
    } while (std::filesystem::exists(new_path));

    return new_path.string();
}


void save_png(const unsigned char* bytes, int width, int height)
{   
                                                //baseline path, if this doesnt exist it will be returned back
    std::string file_path = get_unique_filepath("Resources/Drawings/drawing.png");
       
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