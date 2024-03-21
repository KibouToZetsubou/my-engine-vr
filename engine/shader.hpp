#pragma once

#include <string>

#include "common.hpp"
#include "object.hpp"

class LIB_API Shader : public Object
{
public:
    Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
    ~Shader();

    void render(const glm::mat4 world_matrix) const override;

    bool compile();
protected:
    std::string vertex_shader_source;
    std::string fragment_shader_source;

    unsigned int vertex_shader_id;
    unsigned int fragment_shader_id;
    unsigned int program_id;
};
