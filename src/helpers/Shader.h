//
// Created by ninja on 2/29/2024.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glad/glad.h>
#include "Texture2D.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <map>
#include <vector>

class Shader
{
public:
    GLuint ID;
    typedef std::map<const std::string, GLint> UniformLocations;
    UniformLocations locations;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    // glUseProgram(this)
    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setTexture2D(const std::string& name, GLuint texUnit, const Texture2D& value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};


#endif //LEARNOPENGL_SHADER_H
