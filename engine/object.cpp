#include "object.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "common.hpp"


int Object::next_id = 0;

/**
 * Creates a new instance of Node with the following default parameters:
 *
 * Name: [id], where id is the auto-generated ID.
 */
LIB_API Object::Object()
{
    this->id = Object::next_id++;

    // Set the name as "[<id>]".
    std::stringstream stream;
    stream << '[' << this->get_id() << ']';
    this->name = stream.str();
}

/**
 * Returns the auto-generated ID of this object.
 *
 * @param The ID of this object.
 */
int LIB_API Object::get_id() const
{
    return this->id;
}

/**
 * Adds an object as a child to this object.
 *
 * NOTE: Adding the same object as a child to multiple other objects is undefined behaviour.
 *
 * NOTE: A value of nullptr causes undefined behaviour.
 *
 * NOTE: Adding an object as a child of itself is undefined behaviour.
 */
void LIB_API Object::add_child(const std::shared_ptr<Object> new_child)
{
    this->children.push_back(new_child);
}

/**
 * Returns a copy of the vector containing the pointers to the child objects.
 *
 * Modifying this vector does not affect the children, but modifying the children does affect them.
 *
 * @return A copy of the vector of children.
 */
std::vector<std::shared_ptr<Object>> LIB_API Object::get_children() const
{
    return this->children;
}
/**
 * Returns the local model matrix. This is used by the engine.
 *
 * For objects, the matrix is the identity matrix.
 *
 * @return The model matrix for this object.
 */
glm::mat4 LIB_API Object::get_local_matrix() const
{
    return glm::mat4(1.0f);
}

/**
 * Returns the priority of this object.
 *
 * Objects have a priority of 0.
 *
 * @return The priority of this object.
 */
int LIB_API Object::get_priority() const
{
    return 0;
}

/**
 * Returns the name of this object.
 *
 * @param The name of this object.
 */
std::string LIB_API Object::get_name() const
{
    return this->name;
}

/**
 * @param new_name The new name of this object.
 */
void LIB_API Object::set_name(const std::string new_name)
{
    this->name = new_name;
}
