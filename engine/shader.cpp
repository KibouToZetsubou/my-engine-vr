#include "shader.hpp"

#include <iostream>
#include <map>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

LIB_API Shader::Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
    this->compile(vertex_shader_source, fragment_shader_source);
}

LIB_API Shader::~Shader()
{
    glDeleteProgram(this->program_id);
}

void LIB_API Shader::render(const glm::mat4 world_matrix) const
{
    glUseProgram(this->program_id);

    // TODO: Make the camera settings and the screen ratio configurable
    // Projection matrix
    const glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), 1.333333333333333f, 0.01f, 1000.0f);
    const int uniform_location_matrix = glGetUniformLocation(this->program_id, "projection_matrix");
    glUniformMatrix4fv(uniform_location_matrix, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    // View matrix
    const int uniform_location_view_matrix = glGetUniformLocation(this->program_id, "view_matrix");
    glUniformMatrix4fv(uniform_location_view_matrix, 1, GL_FALSE, glm::value_ptr(world_matrix));

    // Inverse-Transpose of view matrix
    const glm::mat4 inverse_transpose_view = glm::inverseTranspose(world_matrix);
    const int uniform_location_inverse_transpose_view = glGetUniformLocation(this->program_id, "inverse_transpose_view");
    glUniformMatrix4fv(uniform_location_inverse_transpose_view, 1, GL_FALSE, glm::value_ptr(inverse_transpose_view));

    for (auto i = this->floats.begin(); i != this->floats.end(); ++i)
    {
        // TODO: Handle case where the uniform in the map does not exists in the shader.
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        glUniform1f(uniform_location, i->second);
    }

    for (auto i = this->ints.begin(); i != this->ints.end(); ++i)
    {
        // TODO: Handle case where the uniform in the map does not exists in the shader.
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        glUniform1i(uniform_location, i->second);
    }

    for (auto i = this->vec3s.begin(); i != this->vec3s.end(); ++i)
    {
        // TODO: Handle case where the uniform in the map does not exists in the shader.
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        glUniform3fv(uniform_location, 1, glm::value_ptr(i->second));
    }
}

void LIB_API Shader::clear_uniforms()
{
    this->floats.clear();
    this->ints.clear();
    this->vec3s.clear();
}

void LIB_API Shader::set_float(const std::string name, const float value)
{
    this->floats[name] = value;
}

void LIB_API Shader::set_int(const std::string name, const int value)
{
    this->ints[name] = value;
}

void LIB_API Shader::set_vec3(const std::string name, const glm::vec3 value)
{
    this->vec3s[name] = value;
}

void LIB_API Shader::compile(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
    // Compile vertex shader
    const unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source = vertex_shader_source.c_str();
    glShaderSource(vertex_shader_id, 1, (const char**) &vertex_source, nullptr);
    glCompileShader(vertex_shader_id);

    GLint success;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        throw "Failed to compile vertex shader.";
    }

    // Compile fragment shader
    const unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader = fragment_shader_source.c_str();
    glShaderSource(fragment_shader_id, 1, (const char**) &fragment_shader, nullptr);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        throw "Failed to compile fragment shader.";
    }

    // Link the program
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, fragment_shader_id);
    glAttachShader(this->program_id, vertex_shader_id);
    glLinkProgram(this->program_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}
