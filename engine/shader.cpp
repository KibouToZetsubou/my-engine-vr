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

    constexpr float aspect_ratio = 800.0f / 600.0f;
    const glm::mat4 perspective_matrix = glm::perspective(glm::radians(90.0f), aspect_ratio, 0.01f, 1000.0f);

    const int parameter_location_matrix = glGetUniformLocation(this->program_id, "matrix");
    glUniformMatrix4fv(parameter_location_matrix, 1, GL_FALSE, glm::value_ptr(perspective_matrix * world_matrix));

    // TODO: If the rendering works fine, remove this
    // Requires a bound buffer
    /*glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);*/


    /*glBindBuffer(GL_ARRAY_BUFFER, vbo_color); // TODO: Find a way to get the color VBO
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);
    glEnableVertexAttribArray(1);*/
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
