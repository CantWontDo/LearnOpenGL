//
// Created by ninja on 2/29/2024.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include "glad/glad.h"
#include "Texture2D.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    GLuint ID;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    // glUseProgram(this)
    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setTexture2D(const std::string& name, GLuint texUnit, const Texture2D& value) const;
private:

};


#endif //LEARNOPENGL_SHADER_H
