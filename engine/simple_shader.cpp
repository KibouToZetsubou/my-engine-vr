#include "simple_shader.hpp"

LIB_API SimpleShader::SimpleShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 projection;
        uniform mat4 modelview;

        layout(location = 0) in vec3 in_Position;
        layout(location = 1) in vec4 in_Color;

        out vec3 out_Color;
        out float dist;

        void main(void)
        {
            gl_Position = projection * modelview * vec4(in_Position, 1.0f);
            dist = abs(gl_Position.z / 100.0f);
            out_Color = in_Color.rgb;
        }
    )", R"(
        // Fragment shader
        #version 440 core

        in  vec3 out_Color;
        in  float dist;

        out vec4 frag_Output;

        void main(void)
        {
            vec3 fog = vec3(1.0f, 1.0f, 1.0f);
            frag_Output = vec4(mix(out_Color, fog, dist), 1.0f);
        }
    )")
    {}
