#include "simple_shader.hpp"

LIB_API SimpleShader::SimpleShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 matrix;

        layout(location = 0) in vec3 position;
        //layout(location = 1) in vec3 normal;
        //layout(location = 2) in vec2 uv;

        out vec4 color;

        void main(void)
        {
            gl_Position = matrix * vec4(position.x * 0.05f, position.y * 0.05f - 1.0f, position.z * 0.05f, 1.0f);
            color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
        }
    )", R"(
        // Fragment shader
        #version 440 core

        in vec4 color;

        out vec4 fragment;

        void main(void)
        {
            fragment = color;
        }
    )")
    {}
