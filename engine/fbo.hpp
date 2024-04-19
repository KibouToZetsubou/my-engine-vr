#pragma once

#include "common.hpp"

class LIB_API FBO
{
public:
    FBO(const int width, const int height);

    void use() const;
    void use_read() const;
private:
    unsigned int id;
    unsigned int color_buffer_id;
    unsigned int depth_buffer_id;

    int width;
    int height;
};
