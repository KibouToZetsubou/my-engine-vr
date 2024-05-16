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
            normal_eyes = (inverse_transpose_world * vec4(normal, 1.0f)).xyz;
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
        uniform int   number_of_lights;
        //uniform int   light_type[MAX_LIGHTS]; // 0 = Directional; 1 = Point; 2 = Spot
        uniform vec3  light_ambient[MAX_LIGHTS];
        uniform vec3  light_diffuse[MAX_LIGHTS];
        uniform vec3  light_specular[MAX_LIGHTS];
        uniform vec3  light_position[MAX_LIGHTS]; // Light position in eye coordinates
        uniform vec3  light_direction[MAX_LIGHTS]; // Directional, Spot
        uniform float light_radius[MAX_LIGHTS]; // Point, Spot
        uniform float light_cutoff[MAX_LIGHTS]; // Spot
        uniform float light_exponent[MAX_LIGHTS]; // Spot

        in vec3 normal_eyes;
        in vec3 position_eyes;
        in vec2 uv_;

        out vec4 fragment;

        void main(void)
        {
            vec3 total_light_ambient = vec3(0.0f, 0.0f, 0.0f);
            vec3 total_diffuse = vec3(0.0f, 0.0f, 0.0f);
            vec3 total_specular = vec3(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < number_of_lights; ++i)
            {
                const vec3 L = normalize(light_position[i] - position_eyes.xyz);
                const float L_dot_N = dot(L, normalize(normal_eyes));
                const vec3 H = normalize(L + normalize(-position_eyes.xyz));
                const float H_dot_N = dot(H, normalize(normal_eyes));

                total_light_ambient += light_ambient[i];

                if (L_dot_N > 0.0f)
                {
                    total_diffuse += material_diffuse * L_dot_N * light_diffuse[i];
                    total_specular += material_specular * pow(H_dot_N, material_shininess) * light_specular[i];
                }
            }

            const vec3 base_color = material_emission + material_ambient * total_light_ambient + total_diffuse + total_specular;

            if (use_texture)
            {
                fragment = texture(material_texture, uv_) * vec4(base_color, 1.0f);
            }
            else
            {
                fragment = vec4(base_color, 1.0f);
            }

            //fragment = vec4(light_ambient[0], 1.0f);
        }
    )")
{}
