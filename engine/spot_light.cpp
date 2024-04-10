#include "spot_light.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"


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

    //glEnable(GL_LIGHT0 + this->light_id);

    const glm::vec4 light_position(this->direction, 1.0f);
    const glm::vec3 light_direction(0.0f, -0.1f, 0.0f);
    const glm::vec4 ambient(this->ambient_color, 1.0f);
    const glm::vec4 diffuse(this->diffuse_color, 1.0f);
    const glm::vec4 specular(this->specular_color, 1.0f);
    const float constant_attenuation = 1.0f / this->radius;

    const int current_light = Light::get_current_light(this->light_id);

    /*glLightfv(current_light, GL_POSITION, glm::value_ptr(light_position));
    glLightfv(current_light, GL_SPOT_DIRECTION, glm::value_ptr(light_direction));
    glLightfv(current_light, GL_AMBIENT, glm::value_ptr(ambient));
    glLightfv(current_light, GL_DIFFUSE, glm::value_ptr(diffuse));
    glLightfv(current_light, GL_SPECULAR, glm::value_ptr(specular));
    glLightfv(current_light, GL_SPOT_CUTOFF, &this->cutoff);
    glLightf(current_light, GL_CONSTANT_ATTENUATION, constant_attenuation);
    glLightf(current_light, GL_SPOT_EXPONENT, this->exponent);*/
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

/**
 * Sets the radius of the spot light. This is the distance in which the light has an effect.
 *
 * @param new_radius The new radius of this spot light.
 */
void LIB_API SpotLight::set_radius(const float new_radius)
{
    this->radius = new_radius;
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
