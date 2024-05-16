#include "point_light.hpp"

/**
 * Creates a new instance of PointLight with the following default parameters:
 *
 * Radius: 1.0f
 */
LIB_API PointLight::PointLight()
{
    this->set_radius(1.0f);
}

/**
 * Renders the point light.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param view_matrix The world matrix to use to render this object.
 */
void LIB_API PointLight::render(const glm::mat4 view_matrix) const
{
    Node::render(view_matrix);
}

/**
 * Sets the radius of the point light. This is the radius in which the light has an effect.
 *
 * @param new_radius The new radius of this point light.
 */
void LIB_API PointLight::set_radius(const float new_radius)
{
    this->radius = new_radius;
}

float LIB_API PointLight::get_radius() const
{
    return this->radius;
}
