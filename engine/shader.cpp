#include "shader.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

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

    const int parameter_location_matrix = glGetUniformLocation(this->program_id, "matrix");
    glUniformMatrix4fv(parameter_location_matrix, 1, GL_FALSE, glm::value_ptr(world_matrix));

    std::cout << "1" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
    std::cout << "VBO ID: " << this->vbo_vertices << std::endl;
    std::cout << "2" << std::endl;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    /*glBindBuffer(GL_ARRAY_BUFFER, vbo_color); // TODO: Find a way to get the color VBO
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);
    glEnableVertexAttribArray(1);*/
}

bool LIB_API Shader::compile()
{
    // Compile vertex shader
    this->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source = this->vertex_shader_source.c_str();
    glShaderSource(this->vertex_shader_id, 1, (const char**) &vertex_source, nullptr);
    glCompileShader(this->vertex_shader_id);

    GLint success;
    glGetShaderiv(this->vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        std::cout << "[ERROR] Failed to compile vertex shader." << std::endl;

        return false;
    }

    // Compile fragment shader
    this->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader = this->fragment_shader_source.c_str();
    glShaderSource(this->fragment_shader_id, 1, (const char**) &fragment_shader, nullptr);
    glCompileShader(this->fragment_shader_id);

    glGetShaderiv(this->fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        std::cout << "[ERROR] Failed to compile fragment shader." << std::endl;

        return false;
    }

    // Link the program
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, this->fragment_shader_id);
    glAttachShader(this->program_id, this->vertex_shader_id);
    glLinkProgram(this->program_id);

    return true;
}

void LIB_API Shader::set_vbo_vertices(const unsigned int new_vbo_vertices)
{
    this->vbo_vertices = new_vbo_vertices;
}
