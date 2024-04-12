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

    void render(const glm::mat4 view_matrix) const override;

    void clear_uniforms();
    void set_float(const std::string name, const float value);
    void set_int(const std::string name, const int value);
    void set_vec3(const std::string name, const glm::vec3 value);
    void set_bool(const std::string name, const bool value);
    void set_vector_int(const std::string name, const std::vector<int> value);
    void set_vector_vec3(const std::string name, const std::vector<glm::vec3> value);
    void set_vector_float(const std::string name, const std::vector<float> value);
protected:
    void compile(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

    unsigned int program_id;

    std::map<std::string, float> floats;
    std::map<std::string, int> ints;
    std::map<std::string, glm::vec3> vec3s;
    std::map<std::string, bool> bools;
    std::map<std::string, std::vector<int>> vector_ints;
    std::map<std::string, std::vector<glm::vec3>> vector_vec3s;
    std::map<std::string, std::vector<float>> vector_floats;
};
