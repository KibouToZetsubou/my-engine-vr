#include "light.hpp"

/**
 * Creates a new instance of Camera with the following default parameters:
 *
 * Ambient color: 0.0f, 0.0f, 0.0f (Black)
 *
 * Diffuse color: 1.0f, 1.0f, 1.0f (White)
 *
 * Specular color: 1.0f, 1.0f, 1.0f (White)
 *
 * NOTE: The light is automatically activated as soon as it is instantiated.
 */
LIB_API Light::Light()
{
    this->set_ambient_color(glm::vec3(0.0f, 0.0f, 0.0f));
    this->set_diffuse_color(glm::vec3(1.0f, 1.0f, 1.0f));
    this->set_specular_color(glm::vec3(1.0f, 1.0f, 1.0f));
}

/**
 * Returns the priority of this object.
 *
 * Lights have a priority of 100.
 *
 * @return The priority of this object.
 */
int LIB_API Light::get_priority() const
{
    return 100;
}

/**
 * Changes the ambient color property of this light.
 *
 * @param new_color The new ambient color of the light.
 */
void LIB_API Light::set_ambient_color(const glm::vec3 new_color)
{
    this->ambient_color = new_color;
}

/**
 * Used to retrieve the ambient color of the light
 *
 * @return The ambient color of the light
 */
glm::vec3 LIB_API Light::get_ambient_color() const
{
    return this->ambient_color;
}

/**
 * Changes the diffuse color property of this light.
 *
 * @param new_color The new diffuse color of the light.
 */
void LIB_API Light::set_diffuse_color(const glm::vec3 new_color)
{
    this->diffuse_color = new_color;
}

/**
 * Used to retrieve the diffuse color of the light
 *
 * @return The diffuse color of the light
 */
glm::vec3 LIB_API Light::get_diffuse_color() const
{
    return this->diffuse_color;
}

/**
 * Changes the specular color property of this light.
 *
 * @param new_color The new specular color of the light.
 */
void LIB_API Light::set_specular_color(const glm::vec3 new_color)
{
    this->specular_color = new_color;
}

/**
 * Used to retrieve the specular color of the light
 *
 * @return The specular color of the light
 */
glm::vec3 LIB_API Light::get_specular_color() const
{
    return this->specular_color;
}
