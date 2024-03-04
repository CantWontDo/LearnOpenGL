//
// Created by ninja on 3/2/2024.
//

#ifndef LEARNOPENGL_TEXTURE2D_H
#define LEARNOPENGL_TEXTURE2D_H

#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>

class Texture2D
{
public:
    GLuint ID;
    int width;
    int height;
    int numChannels;

    explicit Texture2D(const char* texturePath, bool generateMipMaps = true);

    Texture2D();

    void use() const;

    void use(GLuint texUnit) const;
};

#endif //LEARNOPENGL_TEXTURE2D_H
