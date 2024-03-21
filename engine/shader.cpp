#include "shader.hpp"

#include <GL/glew.h>

LIB_API Shader::Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
    this->vertex_shader_source = vertex_shader_source;
    this->fragment_shader_source = fragment_shader_source;
}

LIB_API Shader::~Shader()
{
    glDeleteProgram(this->program_id);
    glDeleteShader(this->vertex_shader_id);
    glDeleteShader(this->fragment_shader_id);
}

void LIB_API Shader::render(const glm::mat4 world_matrix) const
{
    glUseProgram(this->program_id);
}

bool LIB_API Shader::compile()
{
    // TODO: Implement error handling.

    // Compile vertex shader
    this->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source = this->vertex_shader_source.c_str();
    glShaderSource(this->vertex_shader_id, 1, (const char**) &vertex_source, nullptr);
    glCompileShader(this->vertex_shader_id);

    // Compile fragment shader
    this->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader = this->fragment_shader_source.c_str();
    glShaderSource(this->fragment_shader_id, 1, (const char**) &fragment_shader, nullptr);
    glCompileShader(this->fragment_shader_id);

    // Link the program
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, this->fragment_shader_id);
    glAttachShader(this->program_id, this->vertex_shader_id);

    return true;
}
