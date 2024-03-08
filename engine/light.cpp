#include "light.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>

#include <GL/freeglut.h>

#include "common.hpp"


int Light::next_light_id = 0;

/**
 * Creates a new instance of Camera with the following default parameters:
 *
 * ID: Autogenerated incrementally.
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
    int max_number_of_lights;
    glGetIntegerv(GL_MAX_LIGHTS, &max_number_of_lights);


    Light::next_light_id++;
    this->light_id = Light::next_light_id % max_number_of_lights;

    std::cout << "This light: " << this->light_id << std::endl;

    if (this->light_id >= max_number_of_lights)
    {
        WARNING("Maximum number of lights reached (" << max_number_of_lights << ").");

        return;
    }

    const int current_light = get_current_light(this->light_id);

    this->set_ambient_color(glm::vec3(0.0f, 0.0f, 0.0f));
    this->set_diffuse_color(glm::vec3(1.0f, 1.0f, 1.0f));
    this->set_specular_color(glm::vec3(1.0f, 1.0f, 1.0f));

    glEnable(current_light);
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
 * Changes the diffuse color property of this light.
 *
 * @param new_color The new diffuse color of the light.
 */
void LIB_API Light::set_diffuse_color(const glm::vec3 new_color)
{
    this->diffuse_color = new_color;
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
 * Returns the current light name based on the light id.
 * 
 * @param light_id The id of the light you want the name of. 
 * @return The light name of the light.
 */
int LIB_API Light::get_current_light(const int light_id) const
{
    return GL_LIGHT0 + light_id;
}
