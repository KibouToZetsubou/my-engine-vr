#include "directional_light.hpp"

#include <glm/glm.hpp>

#include "common.hpp"
#include "node.hpp"

/**
 * Creates a new instance of DirectionalLight with the following default parameters:
 *
 * Direction: 0.0f, 1.0f, 0.0f (Down)
 */
LIB_API DirectionalLight::DirectionalLight()
{
    this->set_direction(glm::vec3(0.0f, 1.0f, 0.0f));
}

/**
 * Renders the directional light.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param view_matrix The world matrix to use to render this object.
 */
void LIB_API DirectionalLight::render(const glm::mat4 view_matrix) const
{
    Node::render(view_matrix);
}

/**
 * Changes the direction the light is pointing towards.
 *
 * It is also possible to change the direction by rotating the node using set_rotation.
 *
 * @param new_direction The new direction of the light.
 */
void LIB_API DirectionalLight::set_direction(const glm::vec3 new_direction)
{
    this->direction = new_direction;
}

glm::vec3 LIB_API DirectionalLight::get_direction() const
{
    return this->direction;
}
