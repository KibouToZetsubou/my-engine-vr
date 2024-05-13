#include "skybox_shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#define THROW_IF_NOT_FOUND(uniform) if (uniform == -1) { throw "Uniform not found"; }

LIB_API SkyboxShader::SkyboxShader() : Shader(R"(
   #version 440 core

   uniform mat4 projection;
   uniform mat4 modelview;

   layout(location = 0) in vec3 in_Position;      

   out vec3 texCoord;

   void main(void)
   {
      texCoord = in_Position;
      gl_Position = projection * modelview * vec4(in_Position, 1.0f);            
   }
)", R"(
   #version 440 core
   
   in vec3 texCoord;
   
   // Texture mapping (cubemap):
   layout(binding = 0) uniform samplerCube cubemapSampler;

   out vec4 fragOutput;

   void main(void)
   {       
      fragOutput = texture(cubemapSampler, texCoord);
   }
)")
{}

void LIB_API SkyboxShader::render(const glm::mat4 view_matrix) const {
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
