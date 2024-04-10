#pragma once

// https://www.opengl.org/registry/specs/EXT/texture_filter_anisotropic.txt
#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF

// Fix for OpenGL on Windows
#ifndef GL_CLAMP_TO_EDGE
    #define GL_CLAMP_TO_EDGE 0x812F
#endif

#include <string>

#include <glm/glm.hpp>

#include "common.hpp"
#include "object.hpp"

/**
 * A texture allows placing images on meshes.
 */
class LIB_API Texture : public Object
{
public:
    Texture(const std::string path);
    ~Texture();

    void render(const glm::mat4 view_matrix) const override;
private:
    void* bitmap;
    unsigned int texture_id;
};
