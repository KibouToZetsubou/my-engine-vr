#include "simple_shader.hpp"

LIB_API SimpleShader::SimpleShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 matrix;

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv;

        out vec3 color;

        void main(void)
        {
            gl_Position = matrix * vec4(position, 1.0f);
            color = vec3(1.0f, 0.0f, 1.0f);
        }
    )", R"(
        // Fragment shader
        #version 440 core

        in vec3 color;

        out vec3 fragment;

        void main(void)
        {
            fragment = color;
        }
    )")
    {}
