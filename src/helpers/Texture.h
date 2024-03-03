//
// Created by ninja on 3/2/2024.
//

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>

class Texture
{
public:
    GLuint ID;
    int width;
    int height;
    int numChannels;

    Texture(const char* texturePath, bool generateMipMaps = true);
};

#endif //LEARNOPENGL_TEXTURE_H
