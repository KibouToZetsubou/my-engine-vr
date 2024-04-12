#include "mesh.hpp"

#include <GL/glew.h>

/**
 * Creates a new empty Mesh with the default material and shadow casting activated.
 */
LIB_API Mesh::Mesh()
{
    this->set_material(std::make_shared<Material>());
    this->set_cast_shadows(true);

    this->vao_id = -1;
    this->number_of_faces = 0;
}

/**
 * Renders the mesh.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param view_matrix The world matrix to use to render this object.
 */
void LIB_API Mesh::render(const glm::mat4 view_matrix) const
{
    Node::render(view_matrix);

    if (this->vao_id == -1 || this->number_of_faces == 0)
    {
        return;
    }

    glBindVertexArray(this->vao_id);
    glDrawElements(GL_TRIANGLES, number_of_faces * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
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
void LIB_API Mesh::set_mesh_data(const std::vector<glm::vec3>& new_vertices, const std::vector<uint32_t>& new_faces, const std::vector<glm::vec3>& new_normals, const std::vector<glm::vec2>& new_uvs)
{
    if (this->vao_id != -1)
    {
        glDeleteVertexArrays(1, &this->vao_id);
        glDeleteBuffers(1, &this->vbo_vertices);
        glDeleteBuffers(1, &this->vbo_normals);
        glDeleteBuffers(1, &this->vbo_uvs);
        glDeleteBuffers(1, &this->vbo_faces);
    }

    glGenVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);

    // Vertices
    glGenBuffers(1, &this->vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, new_vertices.size() * sizeof(new_vertices[0]), new_vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Normals
    glGenBuffers(1, &this->vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, new_normals.size()  * sizeof(glm::vec3), new_normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // UVs
    glGenBuffers(1, &this->vbo_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_uvs);
    glBufferData(GL_ARRAY_BUFFER, new_uvs.size()  * sizeof(glm::vec2), new_uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Faces
    glGenBuffers(1, &this->vbo_faces);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, new_faces.size() * sizeof(uint32_t), new_faces.data(), GL_STATIC_DRAW);
    this->number_of_faces = new_faces.size();

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (this->vao_id != -1)
    {
        glDeleteVertexArrays(1, &this->vao_id);
        glDeleteBuffers(1, &this->vbo_vertices);
        glDeleteBuffers(1, &this->vbo_normals);
        glDeleteBuffers(1, &this->vbo_uvs);
        glDeleteBuffers(1, &this->vbo_faces);
    }

    this->number_of_faces = 0;
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
