#include "shader.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

LIB_API Shader::Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
    // TODO: Compile the shader here so we don't have to store the shader source code.
    this->vertex_shader_source = vertex_shader_source;
    this->fragment_shader_source = fragment_shader_source;
}

LIB_API Shader::~Shader()
{
    glDeleteProgram(this->program_id);
}

void LIB_API Shader::render(const glm::mat4 world_matrix) const
{
    glUseProgram(this->program_id);

    // TODO: Make this dynamic
    // Projection matrix
    const glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), 1.333333333333333f, 0.01f, 1000.0f);
    const int uniform_location_matrix = glGetUniformLocation(this->program_id, "projection_matrix");
    glUniformMatrix4fv(uniform_location_matrix, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    // View matrix
    const int uniform_location_view_matrix = glGetUniformLocation(this->program_id, "view_matrix");
    glUniformMatrix4fv(uniform_location_view_matrix, 1, GL_FALSE, glm::value_ptr(world_matrix));

    // Inverse-Transpose of view matrix
    const glm::mat4 inverse_transpose_view = glm::inverse(glm::transpose(world_matrix));
    const int uniform_location_inverse_transpose_view = glGetUniformLocation(this->program_id, "inverse_transpose_view");
    glUniformMatrix4fv(uniform_location_inverse_transpose_view, 1, GL_FALSE, glm::value_ptr(inverse_transpose_view));
}

bool LIB_API Shader::compile()
{
    // Compile vertex shader
    const unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source = this->vertex_shader_source.c_str();
    glShaderSource(vertex_shader_id, 1, (const char**) &vertex_source, nullptr);
    glCompileShader(vertex_shader_id);

    GLint success;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        std::cout << "[ERROR] Failed to compile vertex shader." << std::endl;

        return false;
    }

    // Compile fragment shader
    const unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader = this->fragment_shader_source.c_str();
    glShaderSource(fragment_shader_id, 1, (const char**) &fragment_shader, nullptr);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        std::cout << "[ERROR] Failed to compile fragment shader." << std::endl;

        return false;
    }

    // Link the program
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, fragment_shader_id);
    glAttachShader(this->program_id, vertex_shader_id);
    glLinkProgram(this->program_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return true;
}
