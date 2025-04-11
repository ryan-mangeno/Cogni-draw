#ifndef IMAGE_H
#define IMAGE_H

#include "stb/stb_image_write.h" 
#include <vector>

void save_png(const unsigned char* pixels, int width, int height);

#endif