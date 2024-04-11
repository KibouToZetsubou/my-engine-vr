#include "simple_shader.hpp"

LIB_API SimpleShader::SimpleShader() : Shader(R"(
        // Vertex shader
        #version 440 core

        uniform mat4 projection_matrix;
        uniform mat4 view_matrix;
        uniform mat4 inverse_transpose_world;

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv;

        out vec3 normal_eyes; // The normal position in eye coordinates
        out vec3 position_eyes; // The vertex position in eye coordinates
        out vec2 uv_;

        void main(void)
        {
            normal_eyes = normalize(inverse_transpose_world * vec4(normal, 1.0f)).xyz;
            position_eyes = (view_matrix * vec4(position, 1.0f)).xyz;
            uv_ = uv;

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
        uniform bool use_texture;
        uniform sampler2D material_texture;

        // Light
        // TODO: Support multiple lights (make these arrays)
        uniform int   light_type; // 0 = No light; 1 = Directional; 2 = Point; 3 = Spot
        uniform vec3  light_ambient;
        uniform vec3  light_diffuse;
        uniform vec3  light_specular;
        uniform vec3  light_position; // Light position in eye coordinates
        uniform vec3  light_direction; // Directional, Spot
        uniform float light_radius; // Point, Spot
        uniform float light_cutoff; // Spot
        uniform float light_exponent; // Spot

        in vec3 normal_eyes;
        in vec3 position_eyes;
        in vec2 uv_;

        out vec4 fragment;

        void main(void)
        {
            /*vec3 total_light_ambient = vec3(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < MAX_LIGHTS; ++i)
                if (light_type[i] != 0)
                    total_light_ambient += light_ambient[i];*/

            const vec3 L = normalize(light_position - position_eyes.xyz);
            const float L_dot_N = dot(L, normal_eyes);
            const vec3 H = normalize(L + normalize(-position_eyes.xyz));
            const float N_dot_H = dot(normal_eyes, H);

            // TODO: Find out why the light is so dark
            const vec3 base_color = material_emission +
                material_ambient * light_ambient +
                material_diffuse * L_dot_N * light_diffuse * 64.0f +
                material_specular * pow(N_dot_H, material_shininess) * light_specular * 64.0f;

            if (use_texture)
            {
                fragment = texture(material_texture, uv_) * vec4(base_color, 1.0f);
            }
            else
            {
                fragment = vec4(base_color, 1.0f);
            }
        }
    )")
    {}
