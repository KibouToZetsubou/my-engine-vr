#pragma once

#include "common.hpp"
#include "object.hpp"

class LIB_API FBO //: public Texture
{
public:
    FBO();

    bool render();

    unsigned int get_texture_id();
private:
    unsigned int id;
    unsigned int depth_buffer_id;

    unsigned int texture_id;

    unsigned int size_X;
    unsigned int size_Y;
};
