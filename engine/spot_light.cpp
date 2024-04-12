#include "spot_light.hpp"

#include <glm/glm.hpp>


#include "glm/ext.hpp"

#include "common.hpp"
#include "node.hpp"

/**
 * Creates a new instance of PointLight with the following default parameters:
 *
 * Direction: 0.0f, 1.0f, 0.0f (Down
 *
 * Cutoff: 45.0f
 *
 * Exponent: 8.0f
 *
 * Radius: 1.0f
 */
LIB_API SpotLight::SpotLight()
{
    this->set_direction(glm::vec3(0.0f, 1.0f, 0.0f));
    this->set_cutoff(45.0f);
    this->set_exponent(8.0f); // 0.0f (no attenuation) - 128.0f
    this->set_radius(1.0f);
}

/**
 * Renders the spot light.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param view_matrix The world matrix to use to render this object.
 */
void LIB_API SpotLight::render(const glm::mat4 view_matrix) const
{
    Node::render(view_matrix);
}

/**
 * Sets the cutoff for this spot light.
 *
 * The cutoff is the "size" of the cone of light emitted by the spot light.
 *
 * See: https://gdbooks.gitbooks.io/legacyopengl/content/Chapter5/light_spotlight.html
 *
 * @param new_cutoff The new cutoff gor this spot light.
 */
void LIB_API SpotLight::set_cutoff(const float new_cutoff)
{
    this->cutoff = new_cutoff;
}

float LIB_API SpotLight::get_cutoff() const
{
    return this->cutoff;
}

/**
 * Sets the radius of the spot light. This is the distance in which the light has an effect.
 *
 * @param new_radius The new radius of this spot light.
 */
void LIB_API SpotLight::set_radius(const float new_radius)
{
    this->radius = new_radius;
}

float LIB_API SpotLight::get_radius() const
{
    return this->radius;
}

/**
 * Sets the exponent for this spot light.
 *
 * The exponent is the attenuation of the light inside the cone of light.
 *
 * See: https://gdbooks.gitbooks.io/legacyopengl/content/Chapter5/light_spotlight.html
 *
 * @param new_exponent The new exponent gor this spot light.
 */
void LIB_API SpotLight::set_exponent(const float new_exponent)
{
    this->exponent = new_exponent;
}

float LIB_API SpotLight::get_exponent() const
{
    return this->exponent;
}

/**
 * Changes the direction the light is pointing towards.
 *
 * It is also possible to change the direction by rotating the node using set_rotation.
 *
 * @param new_direction The new direction of the light.
 */
void LIB_API SpotLight::set_direction(const glm::vec3 new_direction)
{
    this->direction = new_direction;
}

glm::vec3 LIB_API SpotLight::get_direction() const
{
    return this->direction;
}
