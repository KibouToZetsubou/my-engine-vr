#include "texture.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>

#include <GL/freeglut.h>
#include <FreeImage.h>

/**
 * Creates a new instance of Texture by loading an image file from the specified path.
 *
 * @param path The path of the image file.
 */
LIB_API Texture::Texture(const std::string path)
{
    /*FIBITMAP* bmp = FreeImage_Load(FreeImage_GetFileType(path.c_str(), 0), path.c_str());

    if (bmp == nullptr)
    {
        // TODO: Handle error.
        ERROR("Failed to load texture \"" + path + "\".");

        this->bitmap = nullptr;

        return;
    }

    this->bitmap = (void*) FreeImage_ConvertTo32Bits(bmp);
    FreeImage_Unload(bmp);

    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    const int width = FreeImage_GetWidth((FIBITMAP*) this->bitmap);
    const int height = FreeImage_GetHeight((FIBITMAP*) this->bitmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits((FIBITMAP*) this->bitmap));*/
}

/**
 * Frees up the memory used by this texture.
 */
LIB_API Texture::~Texture()
{
    /*glDeleteTextures(1, &this->texture_id);

    // Ugly fix for when trying to free an empty bitmap.
    // TODO: Actually handle invalid textures.
    if (this->bitmap != nullptr)
    {
        FreeImage_Unload((FIBITMAP*) this->bitmap);
    }*/
}

/**
 * Renders the texture.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param world_matrix The world matrix to use to render this object.
 */
void LIB_API Texture::render(const glm::mat4 view_matrix) const
{
    /*glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glEnable(GL_TEXTURE_2D);*/
}
