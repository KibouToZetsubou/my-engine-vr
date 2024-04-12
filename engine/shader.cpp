#include "shader.hpp"

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

void LIB_API Shader::render(const glm::mat4 view_matrix) const
{
    glUseProgram(this->program_id);

    // TODO: Make the camera settings and the screen ratio configurable
    // Projection matrix
    const glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), 1.333333333333333f, 0.01f, 1000.0f);
    const int uniform_location_matrix = glGetUniformLocation(this->program_id, "projection_matrix");
    glUniformMatrix4fv(uniform_location_matrix, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    // View matrix
    const int uniform_location_view_matrix = glGetUniformLocation(this->program_id, "view_matrix");
    glUniformMatrix4fv(uniform_location_view_matrix, 1, GL_FALSE, glm::value_ptr(view_matrix));

    // Inverse-Transpose of view matrix
    const glm::mat4 inverse_transpose_world = glm::inverseTranspose(view_matrix);
    const int uniform_location_inverse_transpose_world = glGetUniformLocation(this->program_id, "inverse_transpose_world");
    glUniformMatrix4fv(uniform_location_inverse_transpose_world, 1, GL_FALSE, glm::value_ptr(inverse_transpose_world));

    for (auto i = this->floats.begin(); i != this->floats.end(); ++i)
    {
        // TODO: Handle case where the uniform in the map does not exist in the shader.
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        glUniform1f(uniform_location, i->second);
    }

    for (auto i = this->ints.begin(); i != this->ints.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());

        if (uniform_location == -1)
        {
            throw "Uniform not found";
        }

        glUniform1i(uniform_location, i->second);
    }

    for (auto i = this->vec3s.begin(); i != this->vec3s.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());

        if (uniform_location == -1)
        {
            throw "Uniform not found";
        }

        glUniform3fv(uniform_location, 1, glm::value_ptr(i->second));
    }

    for (auto i = this->bools.begin(); i != this->bools.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());

        if (uniform_location == -1)
        {
            throw "Uniform not found";
        }

        glUniform1i(uniform_location, i->second);
    }

    for (auto i = this->vector_ints.begin(); i != this->vector_ints.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());

        if (uniform_location == -1)
        {
            throw "Uniform not found";
        }

        glUniform1iv(uniform_location, i->second.size(), i->second.data());
    }

    for (auto i = this->vector_vec3s.begin(); i != this->vector_vec3s.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());

        if (uniform_location == -1)
        {
            throw "Uniform not found";
        }

        glUniform3fv(uniform_location, i->second.size(), &(i->second[0].x) ); // Very ugly hack to pass the array
    }

    for (auto i = this->vector_floats.begin(); i != this->vector_floats.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());

        if (uniform_location == -1)
        {
            throw "Uniform not found";
        }

        glUniform1fv(uniform_location, i->second.size(), i->second.data());
    }
}

void LIB_API Shader::clear_uniforms()
{
    // TODO: Also reset the shader's uniforms
    this->floats.clear();
    this->ints.clear();
    this->vec3s.clear();
    this->bools.clear();
    this->vector_ints.clear();
    this->vector_vec3s.clear();
    this->vector_floats.clear();
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

void LIB_API Shader::set_bool(const std::string name, const bool value)
{
    this->bools[name] = value;
}

// PERFORMANCE: We are creating a copy of the vector, which can be slow!
void LIB_API Shader::set_vector_int(const std::string name, const std::vector<int> value)
{
    this->vector_ints[name] = value;
}

// PERFORMANCE: We are creating a copy of the vector, which can be slow!
void LIB_API Shader::set_vector_vec3(const std::string name, const std::vector<glm::vec3> value)
{
    this->vector_vec3s[name] = value;
}

// PERFORMANCE: We are creating a copy of the vector, which can be slow!
void LIB_API Shader::set_vector_float(const std::string name, const std::vector<float> value)
{
    this->vector_floats[name] = value;
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
