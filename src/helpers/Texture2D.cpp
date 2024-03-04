//
// Created by ninja on 3/2/2024.
//

#include "Texture2D.h"

Texture2D::Texture2D(const char *texturePath, bool generateMipMaps)
{
    // creates an id for the texture object
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // texture wrapping options: tell what to do when texCoords are out of 0-1 range
    // GL_REPEAT just repeats the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // filtering options: tell how to interpret texels (pixel on texture)
    // linear filtering averages neighboring texels (smooths out texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(texturePath, &width, &height, &numChannels, 0);

    if(data)
    {
        // rgb format (jpg)
        if(numChannels == 3)
        {

            // first arg is type of texture (doesn't affect bound 1D and 3D textures)
            // second arg is level of mipmap (if providing manual mipmaps), we use 0 since we will always generate
            // third is the format that the texture should be
            // fourth, fifth are dimensions of texture
            // sixth should be zero for legacy reasons
            // seventh is format of actual data of file
            // seventh is type of actual data
            // eight is actual data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height
                            , 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        // rgba format (png)
        else if(numChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height
                    , 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        if(generateMipMaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }
    else
    {
        std::cout << "texture " << ID << " did not load correctly!\n";
    }
}

Texture2D::Texture2D()
{
    ID = 0;
    width = 0;
    height = 0;
    numChannels = 0;
}

void Texture2D::use() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::use(GLuint texUnit) const
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
    use();
}