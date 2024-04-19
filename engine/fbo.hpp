#pragma once

#include "common.hpp"

class LIB_API FBO //: public Texture
{
public:
    FBO();
private:
    unsigned int id;
    unsigned int depth_buffer_id;
};
