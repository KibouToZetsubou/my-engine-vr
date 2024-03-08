#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include <stack>

#include "common.hpp"
#include "mesh.hpp"
#include "node.hpp"
#include "object.hpp"
#include "light.hpp"
#include "material.hpp"

/**
 * Allows loading a scene graph from an OVO file.
 */
class LIB_API OVOParser
{
public:
    static std::shared_ptr<Object> from_file(const std::string file_path);
private:
    static std::pair<std::shared_ptr<Node>, uint32_t>        parse_node_chunk(const uint8_t* chunk_data, const uint32_t chunk_size);
    static std::pair<std::shared_ptr<Mesh>, uint32_t>        parse_mesh_chunk(const uint8_t* chunk_data, const uint32_t chunk_size);
    static std::pair<std::shared_ptr<Material>, std::string> parse_material_chunk(const uint8_t* chunk_data, const uint32_t chunk_size);
    static std::pair<std::shared_ptr<Light>, uint32_t>       parse_light_chunk(const uint8_t* chunk_data, const uint32_t chunk_size);

    static std::string parse_string(const uint8_t* data);

    static std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};
