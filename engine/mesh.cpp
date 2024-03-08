#include "mesh.hpp"
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>

#include <memory>
#include <tuple>
#include <variant>
#include <vector>

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "common.hpp"
#include "node.hpp"

/**
 * Creates a new empty Mesh with the default material and shadow casting activated.
 */
LIB_API Mesh::Mesh()
{
    this->set_material(std::make_shared<Material>());
    this->set_cast_shadows(true);
}

/**
 * Renders the mesh.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param world_matrix The world matrix to use to render this object.
 */
void LIB_API Mesh::render(const glm::mat4 world_matrix) const
{
    Node::render(world_matrix);

    this->material->render(world_matrix);

    for (const auto& face : this->faces)
    {
        glBegin(GL_TRIANGLES);

        const glm::vec3 vertex_0 = this->vertices[std::get<0>(face)];
        const glm::vec3 vertex_1 = this->vertices[std::get<1>(face)];
        const glm::vec3 vertex_2 = this->vertices[std::get<2>(face)];

        const glm::vec3 normal_0 = this->normals[std::get<0>(face)];
        const glm::vec3 normal_1 = this->normals[std::get<1>(face)];
        const glm::vec3 normal_2 = this->normals[std::get<2>(face)];

        const glm::vec2 uv_0 = this->uvs[std::get<0>(face)];
        const glm::vec2 uv_1 = this->uvs[std::get<1>(face)];
        const glm::vec2 uv_2 = this->uvs[std::get<2>(face)];

        glTexCoord2f(uv_0.x, uv_0.y);
        glNormal3f(normal_0.x, normal_0.y, normal_0.z);
        glVertex3f(vertex_0.x, vertex_0.y, vertex_0.z);

        glTexCoord2f(uv_1.x, uv_1.y);
        glNormal3f(normal_1.x, normal_1.y, normal_1.z);
        glVertex3f(vertex_1.x, vertex_1.y, vertex_1.z);

        glTexCoord2f(uv_2.x, uv_2.y);
        glNormal3f(normal_2.x, normal_2.y, normal_2.z);
        glVertex3f(vertex_2.x, vertex_2.y, vertex_2.z);

        glEnd();
    }
}

/**
 * Changes the material applied to this mesh.
 *
 * NOTE: A value of nullptr causes undefined behaviour.
 *
 * @param new_material The new material for the mesh.
 */
void LIB_API Mesh::set_material(const std::shared_ptr<Material> new_material)
{
    this->material = new_material;
}

/**
 * Returns the material used by this mesh.
 *
 * @return The material used by this mesh.
 */
std::shared_ptr<Material> LIB_API Mesh::get_material() const
{
    return this->material;
}

/**
 * Sets the vertex, face, normal and UV data for this mesh.
 *
 * NOTE: Incorrectly sized vectors cause undefined behaviour.
 *
 * @param new_vertices The new vertex data for the mesh.
 * @param new_faces The new face data for the mesh.
 * @param new_normals The new normal data for the mesh.
 * @param new_uvs The new UV data for the mesh.
 */
void LIB_API Mesh::set_mesh_data(const std::vector<glm::vec3> new_vertices, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> new_faces, const std::vector<glm::vec3> new_normals, const std::vector<glm::vec2> new_uvs)
{
    this->vertices = new_vertices;
    this->faces = new_faces;
    this->normals = new_normals;
    this->uvs = new_uvs;
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &this->vertexVbo);
    glDeleteBuffers(1, &this->normalsVbo);
    glDeleteBuffers(1, &this->uvVbo);

}

/**
 * Sets whether this mesh should cast shadows or not.
 *
 * @param new_cast_shadows Whether this mesh should cast shadows.
 */
void LIB_API Mesh::set_cast_shadows(const bool new_cast_shadows)
{
    this->cast_shadows = new_cast_shadows;
}

/**
 * Returns whether this mesh casts shadows or not.
 *
 * @return Whether this mesh casts shadows.
 */
bool LIB_API Mesh::get_cast_shadows() const
{
    return this->cast_shadows;
}
