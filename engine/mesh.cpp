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

    glBindVertexArray(this->vao);

    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, nullptr);
    //std::cout << "Errors: " << glGetError() << std::endl;

    glBindVertexArray(0);

    /*
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVbo);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVbo);
    glNormalPointer(GL_FLOAT, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, this->uvVbo);
    glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
    //glDrawArrays(GL_TRIANGLES, 0, this->vertices.size()); //???

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->facesVbo);
    
    glDrawElements(GL_TRIANGLES, this->vertices.size() * 4, GL_UNSIGNED_INT, nullptr);


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    */
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

    /*glGenBuffers(1, &this->vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVbo);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3),
        this->vertices.data(), GL_STATIC_DRAW);*/
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &this->vertexVbo);
    glDeleteBuffers(1, &this->normalsVbo);
    glDeleteBuffers(1, &this->uvVbo);
    glDeleteBuffers(1, &this->facesVbo);

    //glDeleteVertexArrays(1, &this->vao);
    std::cout << "Destructor " << std::endl;

}

void Mesh::setVAO(unsigned int new_vao)
{
    this->vao = new_vao;
    //std::cout << "setVAO: " << this->vao << std::endl;

}

void Mesh::setVBO(unsigned int vertex, unsigned int normals, unsigned int uvs, unsigned int faces)
{
    this->vertexVbo = vertex;
    this->normalsVbo = normals;
    this->uvVbo = uvs;
    this->facesVbo = faces;
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
