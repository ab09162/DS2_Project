#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ImageLoader.hpp"
#include <iostream>

bool loadImage(const std::string& path, GLuint& textureID, ImVec2& size) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        size = ImVec2((float)width, (float)height);
        
        stbi_image_free(data);
        return true;
    } else {
        std::cerr << "Failed to load image: " << path << std::endl;
        return false;
    }
}
