#include "ovo_parser.hpp"

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#define GLM_ENABLE_EXPERIMENTAL
#pragma warning(disable:4996) // Disable Visual Studio warning

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "common.hpp"
#include "directional_light.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "node.hpp"
#include "object.hpp"
#include "point_light.hpp"
#include "spot_light.hpp"
#include "texture.hpp"

std::unordered_map<std::string, std::shared_ptr<Material>> OVOParser::materials;

/**
* Parses the byte data into a string format.
* 
* @param data The data to parse.
* 
* @return The parsed string.
*/
std::string LIB_API OVOParser::parse_string(const uint8_t* data)
{
    std::string string;

    uint32_t pointer = 0;
    while (data[pointer] != 0x00)
    {
        const char character = (char)data[pointer];

        string += character;

        ++pointer;
    }

    return string;
}

/**
* Loads the scene from a .ovo file and returns its scene root.
*
* @param file_path The path to the .ovo file.
*
* @return The root of the scene.
*/
std::shared_ptr<Object> LIB_API OVOParser::from_file(const std::string file_path)
{
    OVOParser::materials.clear();

    FILE* file = fopen(file_path.c_str(), "rb");

    if (file == nullptr)
    {
        ERROR("Failed to read file \"" + file_path + "\".");
    }

    DEBUG("Loading file \"" << file_path.c_str() << "\" ...");

    std::stack<std::pair<std::shared_ptr<Object>, uint32_t>> hierarchy; // Object, Number of Children

    // Add the scene root
    std::shared_ptr<Node> scene_root = std::make_shared<Node>();
    scene_root->set_name("Scene Root");
    hierarchy.push(std::make_pair(scene_root, 1));

    while (true)
    {
        uint32_t chunk_type;
        uint32_t chunk_size;

        fread(&chunk_type, sizeof(uint32_t), 1, file);

        if (feof(file))
        {
            break;
        }

        fread(&chunk_size, sizeof(uint32_t), 1, file);

        uint8_t* chunk_data = new uint8_t[chunk_size];
        fread(chunk_data, sizeof(uint8_t), chunk_size, file);

        if (chunk_type == 0) // Version
        {
            uint32_t version;
            memcpy(&version, chunk_data, sizeof(uint32_t));

            DEBUG("Version: " << version)
        }
        else if (chunk_type == 1) // Node
        {
            const std::pair<std::shared_ptr<Node>, uint32_t> ret = OVOParser::parse_node_chunk(chunk_data, chunk_size);

            auto& top = hierarchy.top();

            top.first->add_child(ret.first);
            --top.second;

            assert(top.second >= 0);

            hierarchy.push(ret);
        }
        else if (chunk_type == 9) // Material

        {
            const std::pair<std::shared_ptr<Material>, std::string> ret = OVOParser::parse_material_chunk(chunk_data, chunk_size);

            const std::shared_ptr<Material> material = ret.first;
            const std::string material_name = ret.second;

            OVOParser::materials[material_name] = material;
        }
        else if (chunk_type == 16) // Light
        {
            const std::pair<std::shared_ptr<Light>, uint32_t> ret = OVOParser::parse_light_chunk(chunk_data, chunk_size);

            auto& top = hierarchy.top();

            top.first->add_child(ret.first);
            --top.second;

            assert(top.second >= 0);

            hierarchy.push(ret);
        }
        else if (chunk_type == 18) // Mesh
        {
            const std::pair<std::shared_ptr<Mesh>, uint32_t> ret = OVOParser::parse_mesh_chunk(chunk_data, chunk_size);

            auto& top = hierarchy.top();

            top.first->add_child(ret.first);
            --top.second;

            assert(top.second >= 0);

            hierarchy.push(ret);
        }
        else
        {
            WARNING("Unsupported chunk ID " << chunk_type);
        }

        while (hierarchy.size() > 0 && hierarchy.top().second == 0)
        {
            hierarchy.pop();
        }

        delete[] chunk_data;
    }

    fclose(file);

    return scene_root;
}

/**
* Parses a byte chunk into a Node object
*
* @param chunk_data The chunk data to parse.
* @param chunk_size The size of the chunk.
*
* @return The chunk parsed to a Node object.
*/
std::pair<std::shared_ptr<Node>, uint32_t> LIB_API OVOParser::parse_node_chunk(const uint8_t* chunk_data, const uint32_t chunk_size)
{
    std::shared_ptr<Node> node = std::make_shared<Node>();

    uint32_t chunk_pointer = 0;

    // Parse name
    {
        const std::string node_name = OVOParser::parse_string(chunk_data + chunk_pointer);
        chunk_pointer += node_name.length() + 1;
        node->set_name(node_name);
    }

    // Parse matrix
    {
        glm::mat4 matrix;
        memcpy(&matrix, chunk_data + chunk_pointer, sizeof(glm::mat4));
        chunk_pointer += sizeof(glm::mat4);
        node->set_base_matrix(matrix);
    }

    // Parse number of children
    uint32_t number_of_children;
    {
        memcpy(&number_of_children, chunk_data + chunk_pointer, sizeof(uint32_t));
        chunk_pointer += sizeof(uint32_t);
    }

    return std::make_pair(node, number_of_children);
}

/**
* Parses a byte chunk into a Mesh object
*
* @param chunk_data The chunk data to parse.
* @param chunk_size The size of the chunk.
*
* @return The chunk parsed to a Mesh object.
*/
std::pair<std::shared_ptr<Mesh>, uint32_t> LIB_API OVOParser::parse_mesh_chunk(const uint8_t* chunk_data, const uint32_t chunk_size)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    uint32_t chunk_pointer = 0;

    // Name
    {
        const std::string mesh_name = OVOParser::parse_string(chunk_data + chunk_pointer);
        chunk_pointer += mesh_name.length() + 1;
        mesh->set_name(mesh_name);
    }

    // Matrix
    {
        glm::mat4 matrix;
        memcpy(&matrix, chunk_data + chunk_pointer, sizeof(glm::mat4));
        chunk_pointer += sizeof(glm::mat4);
        mesh->set_base_matrix(matrix);
    }

    // Number of children
    uint32_t number_of_children;
    {
        memcpy(&number_of_children, chunk_data + chunk_pointer, sizeof(uint32_t));
        chunk_pointer += sizeof(uint32_t);
    }

    // Target node
    chunk_pointer += OVOParser::parse_string(chunk_data + chunk_pointer).length() + 1;

    // Mesh sub-type
    chunk_pointer += sizeof(uint8_t);

    // Parse material name
    {
        const std::string material_name = OVOParser::parse_string(chunk_data + chunk_pointer);
        chunk_pointer += material_name.length() + 1;

        if (material_name == "[none]")
        {
            // Leave the default material.
        }
        else if (OVOParser::materials.find(material_name) == OVOParser::materials.end())
        {
            WARNING("Out-of-order material loading is not supported.");
        }
        else
        {
            mesh->set_material(OVOParser::materials[material_name]);
        }
    }

    // Mesh radius size
    chunk_pointer += sizeof(float);

    // Mesh bounding box minimum corne
    chunk_pointer += sizeof(glm::vec3);

    // Mesh bounding box maximum corner
    chunk_pointer += sizeof(glm::vec3);

    // Physics
    {
        uint8_t has_physics;
        memcpy(&has_physics, chunk_data + chunk_pointer, sizeof(uint8_t));
        chunk_pointer += sizeof(uint8_t);

        if (has_physics)
        {
            chunk_pointer += 40;

            // Number of hulls
            uint32_t number_of_hulls;
            memcpy(&number_of_hulls, chunk_data + chunk_pointer, sizeof(uint32_t));
            chunk_pointer += sizeof(uint32_t);

            chunk_pointer += 20;

            for (uint32_t i = 0; i < number_of_hulls; ++i)
            {
                uint32_t number_of_hull_vertices;
                memcpy(&number_of_hull_vertices, chunk_data + chunk_pointer, sizeof(uint32_t));
                chunk_pointer += sizeof(uint32_t);

                uint32_t number_of_hull_faces;
                memcpy(&number_of_hull_faces, chunk_data + chunk_pointer, sizeof(uint32_t));
                chunk_pointer += sizeof(uint32_t);

                chunk_pointer += sizeof(glm::vec3) + number_of_hull_vertices * sizeof(glm::vec3) + number_of_hull_faces * sizeof(uint32_t) * 3;
            }
        }
    }

    // Number of LODs
    uint32_t number_of_lods;
    memcpy(&number_of_lods, chunk_data + chunk_pointer, sizeof(uint32_t));
    chunk_pointer += sizeof(uint32_t);

    if (number_of_lods > 1)
    {
        WARNING("Only one LOD is supported (current mesh has " << number_of_lods << "). Using the first one.");
    }

    for (uint32_t i = 0; i < number_of_lods; ++i)
    {
        uint32_t number_of_vertices;
        memcpy(&number_of_vertices, chunk_data + chunk_pointer, sizeof(uint32_t));
        chunk_pointer += sizeof(uint32_t);

        uint32_t number_of_faces;
        memcpy(&number_of_faces, chunk_data + chunk_pointer, sizeof(uint32_t));
        chunk_pointer += sizeof(uint32_t);

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        for (uint32_t j = 0; j < number_of_vertices; ++j)
        {
            glm::vec3 vertex;
            memcpy(&vertex, chunk_data + chunk_pointer, sizeof(glm::vec3));
            chunk_pointer += sizeof(glm::vec3);
            vertices.push_back(vertex);

            uint32_t normal_raw;
            memcpy(&normal_raw, chunk_data + chunk_pointer, sizeof(uint32_t));
            chunk_pointer += sizeof(uint32_t);
            const glm::vec3 normal = glm::vec3(glm::unpackSnorm3x10_1x2(normal_raw));
            normals.push_back(normal);

            uint32_t uv_raw;
            memcpy(&uv_raw, chunk_data + chunk_pointer, sizeof(uint32_t));
            chunk_pointer += sizeof(uint32_t);
            const glm::vec2 uv = glm::unpackHalf2x16(uv_raw);
            uvs.push_back(uv);

            chunk_pointer += sizeof(uint32_t);
        }

        std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> faces;
        std::vector<uint32_t> face_vector;
        for (uint32_t j = 0; j < number_of_faces; ++j)
        {
            uint32_t face_0;
            memcpy(&face_0, chunk_data + chunk_pointer, sizeof(uint32_t));
            chunk_pointer += sizeof(uint32_t);

            uint32_t face_1;
            memcpy(&face_1, chunk_data + chunk_pointer, sizeof(uint32_t));
            chunk_pointer += sizeof(uint32_t);

            uint32_t face_2;
            memcpy(&face_2, chunk_data + chunk_pointer, sizeof(uint32_t));
            chunk_pointer += sizeof(uint32_t);

            const auto face = std::make_tuple(face_0, face_1, face_2);
            face_vector.push_back(face_0);
            face_vector.push_back(face_1);
            face_vector.push_back(face_2);
            faces.push_back(face);
        }

        mesh->set_mesh_data(vertices, faces, normals, uvs);



        /*
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        */



        unsigned int vertexVbo;

        //glEnableClientState(GL_VERTEX_ARRAY);

        glGenBuffers(1, &vertexVbo);
        glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
            vertices.data(), GL_STATIC_DRAW);
        //glEnableVertexAttribArray(0);


        unsigned int normalsVbo;

        //glEnableClientState(GL_NORMAL_ARRAY);

        glGenBuffers(1, &normalsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, normalsVbo);

        glBufferData(GL_ARRAY_BUFFER, normals.size()  * sizeof(glm::vec3),
            normals.data(), GL_STATIC_DRAW);
        //glEnableVertexAttribArray(0);

        unsigned int uvsVbo;

        //glEnableClientState(GL_VERTEX_ARRAY);

        glGenBuffers(1, &uvsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, uvsVbo);

        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2),
            uvs.data(), GL_STATIC_DRAW);
        //glEnableVertexAttribArray(0);

        unsigned int facesVbo;

        //glEnableClientState(GL_VERTEX_ARRAY);

        glGenBuffers(1, &facesVbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesVbo);
        //std::cout << "Number of faces: " << faces.data() << std::endl;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_vector.size() * sizeof(unsigned int),
            face_vector.data(), GL_STATIC_DRAW);
        //glEnableVertexAttribArray(0);

        //mesh->setVAO(vao);
        mesh->setVBO(vertexVbo, normalsVbo, uvsVbo, facesVbo);






        std::cout << "Errors: " << glGetError() << std::endl;
        
        //glBindVertexArray(0);
        // We only consider the first LOD.
        break;
    }


    return std::make_pair(mesh, number_of_children);
}

/**
* Parses a byte chunk into a Material object
*
* @param chunk_data The chunk data to parse.
* @param chunk_size The size of the chunk.
*
* @return The chunk parsed to a Material object.
*/
std::pair<std::shared_ptr<Material>, std::string> LIB_API OVOParser::parse_material_chunk(const uint8_t* chunk_data, const uint32_t chunk_size)
{
    std::shared_ptr<Material> material = std::make_shared<Material>();

    uint32_t chunk_pointer = 0;

    // Material name
    {
        std::string material_name = OVOParser::parse_string(chunk_data + chunk_pointer);
        chunk_pointer += material_name.length() + 1;
        material->set_name(material_name);
    }

    // Emission
    {
        glm::vec3 emission;
        memcpy(&emission, chunk_data + chunk_pointer, sizeof(glm::vec3));
        chunk_pointer += sizeof(glm::vec3);
        material->set_emission_color(emission);
    }

    // Albedo
    glm::vec3 albedo;
    {
        memcpy(&albedo, chunk_data + chunk_pointer, sizeof(glm::vec3));
        chunk_pointer += sizeof(glm::vec3);
    }

    // Roughness
    float roughness;
    {
        memcpy(&roughness, chunk_data + chunk_pointer, sizeof(float));
        chunk_pointer += sizeof(float);
    }

    // Metalness
    chunk_pointer += sizeof(float);

    // Transparency
    chunk_pointer += sizeof(float);

    // Texture name
    {
        std::string texture_name = OVOParser::parse_string(chunk_data + chunk_pointer);
        chunk_pointer += texture_name.length() + 1;

        if (texture_name != "[none]")
        {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(texture_name);
            material->set_texture(texture);
        }
    }

    // Normal map name
    chunk_pointer += OVOParser::parse_string(chunk_data + chunk_pointer).length() + 1;

    // Height map name
    chunk_pointer += OVOParser::parse_string(chunk_data + chunk_pointer).length() + 1;

    // Roughness map name
    chunk_pointer += OVOParser::parse_string(chunk_data + chunk_pointer).length() + 1;

    // Metalness map name
    chunk_pointer += OVOParser::parse_string(chunk_data + chunk_pointer).length() + 1;

    material->set_ambient_color(albedo);
    material->set_specular_color(albedo);
    material->set_diffuse_color(albedo);
    material->set_shininess((1.0f - std::sqrt(roughness)) * 128.0f);

    return std::make_pair(material, material->get_name());
}

/**
* Parses a byte chunk into a Light object
*
* @param chunk_data The chunk data to parse.
* @param chunk_size The size of the chunk.
*
* @return The chunk parsed to a Light object.
*/
std::pair<std::shared_ptr<Light>, uint32_t> LIB_API OVOParser::parse_light_chunk(const uint8_t* chunk_data, const uint32_t chunk_size)
{
    uint32_t chunk_pointer = 0;

    // Light name
    std::string light_name;
    {
        light_name = OVOParser::parse_string(chunk_data + chunk_pointer);
        chunk_pointer += light_name.length() + 1;
    }

    // Matrix
    glm::mat4 matrix;
    {
        memcpy(&matrix, chunk_data + chunk_pointer, sizeof(glm::mat4));
        chunk_pointer += sizeof(glm::mat4);
    }

    // Number of children
    uint32_t number_of_children;
    {
        memcpy(&number_of_children, chunk_data + chunk_pointer, sizeof(uint32_t));
        chunk_pointer += sizeof(uint32_t);
    }

    // Target node
    chunk_pointer += OVOParser::parse_string(chunk_data + chunk_pointer).length() + 1;

    // Light subtype
    // 0: Point
    // 1: Directional
    // 2: Spot
    uint8_t subtype;
    {
        memcpy(&subtype, chunk_data + chunk_pointer, sizeof(uint8_t));
        chunk_pointer += sizeof(uint8_t);
    }

    // Light color
    glm::vec3 color;
    {
        memcpy(&color, chunk_data + chunk_pointer, sizeof(glm::vec3));
        chunk_pointer += sizeof(glm::vec3);
    }

    // Influence radius
    float radius;
    {
        memcpy(&radius, chunk_data + chunk_pointer, sizeof(float));
        chunk_pointer += sizeof(float);
    }

    // Direction
    glm::vec3 direction;
    {
        memcpy(&direction, chunk_data + chunk_pointer, sizeof(glm::vec3));
        chunk_pointer += sizeof(glm::vec3);
    }

    // Cutoff
    float cutoff;
    {
        memcpy(&cutoff, chunk_data + chunk_pointer, sizeof(float));
        chunk_pointer += sizeof(float);
    }

    // Exponent
    float spot_exponent;
    {
        memcpy(&spot_exponent, chunk_data + chunk_pointer, sizeof(float));
        chunk_pointer += sizeof(float);
    }

    if (subtype == 0) // Point
    {
        std::shared_ptr<PointLight> light = std::make_shared<PointLight>();

        light->set_name(light_name);
        light->set_base_matrix(matrix);
        light->set_diffuse_color(color);
        light->set_specular_color(color);
        light->set_radius(radius / 1000.0f); // Arbitrary

        return std::make_pair(light, number_of_children);
    }
    else if (subtype == 1) // Directional
    {
        std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();

        light->set_name(light_name);
        light->set_base_matrix(matrix);
        light->set_diffuse_color(color);
        light->set_specular_color(color);
        light->set_direction(direction);

        return std::make_pair(light, number_of_children);
    }
    else if (subtype == 2) // Spot
    {
        std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>();

        light->set_name(light_name);
        light->set_base_matrix(matrix);
        light->set_diffuse_color(color);
        light->set_specular_color(color);
        light->set_cutoff(cutoff);
        light->set_radius(radius);
        light->set_exponent(spot_exponent);
        light->set_direction(direction);

        return std::make_pair(light, number_of_children);
    }
    else
    {
        WARNING("Unknown light subtype: " << (uint32_t)subtype << ". Defaulting to a point light.");

        return std::make_pair(std::make_shared<PointLight>(), 0);
    }
}
