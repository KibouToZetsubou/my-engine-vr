#include "simple_shader.hpp"

LIB_API SimpleShader::SimpleShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 projection_matrix;
        uniform mat4 view_matrix;
        uniform mat4 inverse_transpose_view;

        // Material
        uniform vec3 material_emission;
        uniform vec3 material_ambient;
        uniform vec3 material_diffuse;
        uniform vec3 material_specular;
        uniform float material_shininess;

        // Light
        uniform int light_type; // 0 = Directional; 1 = Point; 2 = Spot
        uniform vec3 light_ambient;
        uniform vec3 light_diffuse;
        uniform vec3 light_specular;
        uniform vec3 light_direction; // Directional, Spot
        uniform float light_radius; // Point, Spot
        uniform float light_cutoff; // Spot
        uniform float light_exponent; // Spot

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        //layout(location = 2) in vec2 uv;

        out vec4 view_normal; // The normal position in world coordinates
        out vec4 position_eyes; // The vertex position in eye coordinates

        void main(void)
        {
            //view_normal = normalize(view_matrix * vec4(normal, 1.0f));
            //view_normal = normalize(inverse_transpose_view * vec4(normal, 1.0f));
            view_normal = vec4(normal, 1.0f);
            position_eyes = view_matrix * vec4(position, 1.0f);

            gl_Position = projection_matrix * view_matrix * vec4(position, 1.0f);
        }
    )", R"(
        // Fragment shader
        #version 440 core

        in vec4 view_normal;
        in vec4 position_eyes;

        out vec4 fragment;

        void main(void)
        {
            fragment = view_normal;
        }
    )")
    {}
