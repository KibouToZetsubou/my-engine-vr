#include "skybox_shader.hpp"

LIB_API SkyboxShader::SkyboxShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 projection_matrix;
        uniform mat4 view_matrix;

        layout(location = 0) in vec3 in_Position;      

        out vec3 texCoord;

        void main(void)
        {
            texCoord = in_Position;
            gl_Position = projection_matrix * view_matrix * vec4(in_Position, 1.0f);            
        }
    )", R"(
        // Fragment shader
        #version 440 core
   
        in vec3 texCoord;
   
        layout(binding = 0) uniform samplerCube cubemapSampler;

        out vec4 fragOutput;

        void main(void)
        {       
            fragOutput = texture(cubemapSampler, texCoord);
        }
    )")
{}
