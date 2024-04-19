#pragma once

#include "common.hpp"
#include "shader.hpp"

class LIB_API PassthroughShader : public Shader
{
public:
    PassthroughShader();

    void render(const glm::mat4 view_matrix) const override;
};
