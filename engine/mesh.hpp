#pragma once

#include <memory>
#include <vector>

#include "common.hpp"
#include "material.hpp"
#include "node.hpp"

/**
 * A mesh represents a shape that can be rendered inside a scene.
 */
class LIB_API Mesh : public Node
{
public:
    Mesh(const std::vector<glm::vec3>& new_vertices,
        const std::vector<uint32_t>& new_faces,
        const std::vector<glm::vec3>& new_normals,
        const std::vector<glm::vec2>& new_uvs);

    ~Mesh();

    void render(const glm::mat4 view_matrix) const override;

    void set_material(const std::shared_ptr<Material> new_material);
    std::shared_ptr<Material> get_material() const;

    void set_cast_shadows(const bool new_cast_shadows);
    bool get_cast_shadows() const;
private:
    std::shared_ptr<Material> material;
    int number_of_faces;

    unsigned int vbo_vertices;
    unsigned int vbo_normals;
    unsigned int vbo_uvs;
    unsigned int vbo_faces;
    unsigned int vao_id;

    bool cast_shadows;
};
