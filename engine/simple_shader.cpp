#include "simple_shader.hpp"

LIB_API SimpleShader::SimpleShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 projection_matrix;
        uniform mat4 view_matrix;
        uniform mat4 inverse_transpose_view;

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

        #define MAX_LIGHTS 8

        // Material
        uniform vec3 material_emission;
        uniform vec3 material_ambient;
        uniform vec3 material_diffuse;
        uniform vec3 material_specular;
        uniform float material_shininess;

        // Light
        uniform int   light_type[MAX_LIGHTS]; // 0 = No light; 1 = Directional; 2 = Point; 3 = Spot
        uniform vec3  light_ambient[MAX_LIGHTS];
        uniform vec3  light_diffuse[MAX_LIGHTS];
        uniform vec3  light_specular[MAX_LIGHTS];
        uniform vec3  light_position[MAX_LIGHTS];
        uniform vec3  light_direction[MAX_LIGHTS]; // Directional, Spot
        uniform float light_radius[MAX_LIGHTS]; // Point, Spot
        uniform float light_cutoff[MAX_LIGHTS]; // Spot
        uniform float light_exponent[MAX_LIGHTS]; // Spot

        in vec4 view_normal;
        in vec4 position_eyes;

        out vec4 fragment;

        void main(void)
        {
            vec3 total_light_ambient = vec3(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < MAX_LIGHTS; ++i)
                if (light_type[i] != 0)
                    total_light_ambient += light_ambient[i];

            //const vec3 base_color = material_emission + material_ambient * total_light_ambient;
            const vec3 base_color = material_diffuse;

            fragment = vec4(base_color, 1.0f);
        }
    )")
    {}
