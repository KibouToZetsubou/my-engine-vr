#include "leap_shader.hpp"

LIB_API LeapShader::LeapShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 projection;
        uniform mat4 modelview;

        layout(location = 0) in vec3 in_Position;      

        void main(void)
        {
            gl_Position = projection * modelview * vec4(in_Position, 1.0f);            
        }
    )", R"(
        // Fragment shader
        #version 440 core
   
        uniform vec3 color;   
        out vec4 frag_Output;
   
        void main(void)
        {      
           frag_Output = vec4(color, 1.0f);
        }
    )")
{}
