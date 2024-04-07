#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "common.hpp"
#include "object.hpp"

class LIB_API Shader : public Object
{
public:
    Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
    ~Shader();

    void render(const glm::mat4 world_matrix) const override;

    // TODO: Compile the shader in the constructor.
    // TODO: Throw an exception if initialization fails.
    bool compile();

    void clear_uniforms();
    void set_float(const std::string name, const float value);
    void set_int(const std::string name, const int value);
    void set_vec3(const std::string name, const glm::vec3 value);
protected:
    // TODO: Do not store the source code.
    std::string vertex_shader_source;
    std::string fragment_shader_source;

    unsigned int program_id;

    std::map<std::string, float> floats;
    std::map<std::string, int> ints;
    std::map<std::string, glm::vec3> vec3s;
};
