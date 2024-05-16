#include "passthrough_shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#define THROW_IF_NOT_FOUND(uniform) if (uniform == -1) { throw "Uniform not found"; }

LIB_API PassthroughShader::PassthroughShader() : Shader(R"(
        #version 440 core

       // Uniforms:
       uniform mat4 projection;
       uniform mat4 modelview;

       // Attributes:
       layout(location = 0) in vec2 in_Position;
       layout(location = 2) in vec2 in_TexCoord;

       // Varying:
       out vec2 texCoord;

       void main(void)
       {
          gl_Position = projection * modelview * vec4(in_Position, 0.0f, 1.0f);
          texCoord = in_TexCoord;
       }
    )", R"(
        #version 440 core

        in vec2 texCoord;

        uniform vec4 color;

        out vec4 fragOutput;

        // Texture mapping:
        layout(binding = 0) uniform sampler2D texSampler;

        void main(void)
        {
            // Texture element:
            vec4 texel = texture(texSampler, texCoord);

            // Final color:
            fragOutput = color * texel;
        }
    )")
{}

// TODO: I don't really like this hack... discuss with Piazza - BMPG
void LIB_API PassthroughShader::render(const glm::mat4 view_matrix) const {
    for (auto i = this->floats.begin(); i != this->floats.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform1f(uniform_location, i->second);
    }

    for (auto i = this->ints.begin(); i != this->ints.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform1i(uniform_location, i->second);
    }

    for (auto i = this->vec3s.begin(); i != this->vec3s.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform3fv(uniform_location, 1, glm::value_ptr(i->second));
    }

    for (auto i = this->vec4s.begin(); i != this->vec4s.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform4fv(uniform_location, 1, glm::value_ptr(i->second));
    }

    for (auto i = this->bools.begin(); i != this->bools.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform1i(uniform_location, i->second);
    }

    for (auto i = this->vector_ints.begin(); i != this->vector_ints.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform1iv(uniform_location, i->second.size(), i->second.data());
    }

    for (auto i = this->vector_vec3s.begin(); i != this->vector_vec3s.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform3fv(uniform_location, i->second.size(), &(i->second[0].x)); // Very ugly hack to pass the array
    }

    for (auto i = this->vector_floats.begin(); i != this->vector_floats.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniform1fv(uniform_location, i->second.size(), i->second.data());
    }

    for (auto i = this->mat4s.begin(); i != this->mat4s.end(); ++i)
    {
        const int uniform_location = glGetUniformLocation(this->program_id, i->first.c_str());
        THROW_IF_NOT_FOUND(uniform_location);
        glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(i->second));
    }
}
