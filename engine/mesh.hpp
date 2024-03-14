#pragma once

#include <memory>
#include <tuple>

#include "common.hpp"
#include "material.hpp"
#include "node.hpp"

/**
 * A mesh represents a shape that can be rendered inside a scene.
 */
class LIB_API Mesh : public Node
{
public:
    Mesh();

    void render(const glm::mat4 world_matrix) const override;

    void set_material(const std::shared_ptr<Material> new_material);
    std::shared_ptr<Material> get_material() const;

    void set_cast_shadows(const bool new_cast_shadows);
    bool get_cast_shadows() const;

    void set_mesh_data(
        const std::vector<glm::vec3> new_vertices,
        const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> new_faces,
        const std::vector<glm::vec3> new_normals,
        const std::vector<glm::vec2> new_uvs);
    ~Mesh();

    void setVAO(unsigned int new_vao);
    void setVBO(unsigned int vertex, unsigned int normals, unsigned int uvs, unsigned int faces);


private:
    std::shared_ptr<Material> material;
    std::vector<glm::vec3> vertices;
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> faces;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    unsigned int vertexVbo = 0;
    unsigned int normalsVbo = 0;
    unsigned int uvVbo = 0;
    unsigned int facesVbo = 0;

    unsigned int vao = 0;

    bool cast_shadows;
};
