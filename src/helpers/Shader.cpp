//
// Created by ninja on 2/29/2024.
//

#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    // stores source code of vertex and fragment shaders
    std::string vertexCode {};
    std::string fragmentCode {};

    // file streams read from shader files to get source code
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // allows ifstreams to throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open file path
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);

        // read from buffer into string stream, copy to source code
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(std::ifstream::failure &e)
    {
        std::cout << "ERROR: SHADER FILE NOT SUCCESSFULLY READ\n";
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex;
    GLuint fragment;

    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ID = glCreateProgram();

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // amount of uniforms in shader
    GLint locationCount;
    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &locationCount);

    std::vector<std::string> auxUniforms;
    for(int i = 0; i < locationCount; i++)
    {
        int length;
        char buffer[100];
        GLint size;
        GLenum type;

        glGetActiveUniform(ID, GLuint(i), sizeof(buffer)-1, &length, &size, &type, buffer);
        buffer[length] = 0;
        std::string name = std::string(buffer);

        GLint location = glGetUniformLocation(ID, name.c_str());

        locations[name] = location;
    }

    std::cout << locationCount << '\n';
}

void Shader::use() const
{
    glUseProgram(ID);
}

// only works for uniforms being used in shader
void Shader::setBool(const std::string& name, bool value) const
{
    if(locations.contains(name))
    {
        glUniform1i(locations.at(name), (int) value);
    }
    else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setFloat(const std::string &name, float value) const
{
    if(locations.contains(name))
    {
        glUniform1f(locations.at(name), value);
    }
    else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setInt(const std::string &name, int value) const
{
    if(locations.contains(name))
    {
        glUniform1i(locations.at(name), value);
    }
    else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setTexture2D(const std::string &name, const GLuint texUnit, const Texture2D& value) const
{
    if(locations.contains(name))
    {
        value.use(texUnit);
        setInt(name, (int)texUnit);
    }
    else
    {
        //std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    if(locations.contains(name))
    {
        glUniform2fv(locations.at(name), 1, &value[0]);
    }
    else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setVec2(const std::string &name, float x, float y) const
{
    if(locations.contains(name))
    {
        glUniform2f(locations.at(name), x, y);
    }
    else
    {
        std::cout << name << " does not exist!\n";
    }
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

// only works for uniforms being used in shader
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    if (locations.contains(name))
    {
        glUniform3fv(locations.at(name), 1, &value[0]);
    } else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    if (locations.contains(name))
    {
        glUniform3f(locations.at(name), x, y, z);
    } else
    {
        std::cout << name << " does not exist!\n";
    }
}
// only works for uniforms being used in shader
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    if (locations.contains(name))
    {
        glUniform4fv(locations.at(name), 1, &value[0]);
    } else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    if (locations.contains(name))
    {
        glUniform4f(locations.at(name), x, y, z, w);
    } else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    if(locations.contains(name))
    {
        glUniformMatrix2fv(locations.at(name), 1, GL_FALSE, &mat[0][0]);
    }
    else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    if (locations.contains(name))
    {
        glUniformMatrix3fv(locations.at(name), 1, GL_FALSE, &mat[0][0]);
    } else
    {
        std::cout << name << " does not exist!\n";
    }
}

// only works for uniforms being used in shader
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    if (locations.contains(name))
    {
        glUniformMatrix4fv(locations.at(name), 1, GL_FALSE, glm::value_ptr(mat));
    } else
    {
        std::cout << name << " does not exist!\n";
    }
}