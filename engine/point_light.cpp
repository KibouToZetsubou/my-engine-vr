#include "point_light.hpp"

//#define GLM_ENABLE_EXPERIMENTAL
//#include <GL/glew.h>

//#include <GL/freeglut.h>
#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "common.hpp"
#include "node.hpp"

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

    /*glEnable(GL_LIGHT0 + this->light_id);

    const glm::vec4 light_position(0.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 ambient(this->ambient_color, 1.0f);
    const glm::vec4 diffuse(this->diffuse_color, 1.0f);
    const glm::vec4 specular(this->specular_color, 1.0f);
    constexpr float cutoff = 180.0f;
    const float constant_attenuation = 1.0f / this->radius;

    const int current_light = Light::get_current_light(this->light_id);

    glLightfv(current_light, GL_POSITION, glm::value_ptr(light_position));
    glLightfv(current_light, GL_AMBIENT, glm::value_ptr(ambient));
    glLightfv(current_light, GL_DIFFUSE, glm::value_ptr(diffuse));
    glLightfv(current_light, GL_SPECULAR, glm::value_ptr(specular));
    glLightfv(current_light, GL_SPOT_CUTOFF, &cutoff);
    glLightf(current_light, GL_CONSTANT_ATTENUATION, constant_attenuation);*/
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
