#pragma once

#include "common.hpp"
#include "shader.hpp"

class LIB_API SkyboxShader : public Shader
{
public:
    SkyboxShader();

    void render(const glm::mat4 view_matrix) const override;
};
