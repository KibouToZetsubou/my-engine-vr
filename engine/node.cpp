#include "node.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>

#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.hpp"

/**
 * Creates a new instance of Node with the following default parameters:
 *
 * Base matrix: Identity
 *
 * Position: 0.0f, 0.0f, 0.0f
 *
 * Rotation: 0.0f, 0.0f, 0.0f
 *
 * Scale: 1.0f, 1.0f, 1.0f
 */
LIB_API Node::Node()
{
    this->set_base_matrix(glm::mat4(1.0f));
    this->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    this->set_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    this->set_scale(glm::vec3(1.0f, 1.0f, 1.0f));
}

/**
 * Sets the base model matrix for this node.
 *
 * @param new_base_matrix The new base matrix.
 */
void LIB_API Node::set_base_matrix(const glm::mat4 new_base_matrix)
{
    this->base_matrix = new_base_matrix;
}

/**
 * Sets the position offset from the base matrix.
 *
 * @param new_position The new position offset.
 */
void LIB_API Node::set_position(const glm::vec3 new_position)
{
    this->position = new_position;
}

/**
 * Sets the rotation offset from the base matrix.
 *
 * @param new_rotation The new rotation offset.
 */
void LIB_API Node::set_rotation(const glm::vec3 new_rotation)
{
    this->rotation = new_rotation;
}

/**
 * Sets the scale offset from the base matrix.
 *
 * @param new_scale The new scale offset.
 */
void LIB_API Node::set_scale(const glm::vec3 new_scale)
{
    this->scale = new_scale;
}

/**
 * Returns the position offset from the base matrix.
 *
 * @return The position offset.
 */
glm::vec3 LIB_API Node::get_position() const
{
    return this->position;
}

/**
 * Returns the rotation offset from the base matrix.
 *
 * @return The rotation offset.
 */
glm::vec3 LIB_API Node::get_rotation() const
{
    return this->rotation;
}

/**
 * Returns the scale offset from the base matrix.
 *
 * @return The scale offset.
 */
glm::vec3 LIB_API Node::get_scale() const
{
    return this->scale;
}

/**
 * Returns the local model matrix. This is used by the engine.
 *
 * The local matrix is calculated in the following order:
 *
 *  1. Base matrix
 *  2. Scale matrix
 *  3. X rotation matrix
 *  4. Y rotation matrix
 *  5. Z rotation matrix
 *  6. Translation matrix
 *
 * @return The model matrix for this node.
 */
glm::mat4 LIB_API Node::get_local_matrix() const
{
    const glm::mat4 model_position_matrix = glm::translate(glm::mat4(1.0f), this->position);
    const glm::mat4 model_rotation_matrix =
        glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 model_scale_matrix = glm::scale(glm::mat4(1.0f), this->scale);

    const glm::mat4 offset_matrix = model_position_matrix * model_rotation_matrix * model_scale_matrix;

    return offset_matrix * this->base_matrix;
}

/**
 * Renders the node. Nodes do not actually have an appearance, instead they are supposed to be used as empty parents or as base class for other nodes.
 *
 * This function just loads the model matrix for this node.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param world_matrix The world matrix to use to render this object.
 */
void LIB_API Node::render(const glm::mat4 world_matrix) const
{
    /*glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(world_matrix));*/
}
