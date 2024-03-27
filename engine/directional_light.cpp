#include "directional_light.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glm/gtx/string_cast.hpp>

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
 * @param world_matrix The world matrix to use to render this object.
 */
void LIB_API DirectionalLight::render(const glm::mat4 world_matrix) const
{
    Node::render(world_matrix);

    //glEnable(GL_LIGHT0 + this->light_id);

    //const glm::vec4 light_position(this->direction, 0.0f);
    const glm::vec4 light_position(0.0f, 100.0f, 0.0f, 0.0f);
    //const glm::vec4 light_position(this->get_position(), 0.0f);
    const glm::vec4 ambient(this->ambient_color, 1.0f);
    const glm::vec4 diffuse(this->diffuse_color, 1.0f);
    const glm::vec4 specular(this->specular_color, 1.0f);

    const int current_light = Light::get_current_light(this->light_id);

    /*glLightfv(current_light, GL_POSITION, glm::value_ptr(light_position));
    glLightfv(current_light, GL_AMBIENT, glm::value_ptr(ambient));
    glLightfv(current_light, GL_DIFFUSE, glm::value_ptr(diffuse));
    glLightfv(current_light, GL_SPECULAR, glm::value_ptr(specular));*/
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
