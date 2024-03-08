#include "ortho_camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>

#include <GL/glew.h>

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.hpp"
#include "node.hpp"

/**
 * Renders the camera.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param world_matrix The world matrix to use to render this object.
 */
void LIB_API OrthoCamera::render(const glm::mat4 world_matrix) const
{
    if (!this->is_active)
    {
        return;
    }

    Node::render(world_matrix);

    const float width = static_cast<float>(this->window_width);
    const float height = static_cast<float>(this->window_height);

    const float max = std::max(width, height);

    const float w = (width / max) * this->zoom;
    const float h = (height / max) * this->zoom;

    const glm::mat4 ortho_matrix = glm::ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f, this->near_clipping, this->far_clipping);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(ortho_matrix));
}

/**
* Sets the value of the camera zoom.
* 
* @param new_zoom The value of the camera zoom.
*/
void LIB_API OrthoCamera::set_zoom(float new_zoom)
{
    if(new_zoom < 0.0f)
    {
        new_zoom = 0.0f; //To prevent funny loop around
    }

    this->zoom = new_zoom;
}

/**
* Returns the value of the camera zoom.
*
* @return The value of the camera zoom.
*/
float LIB_API OrthoCamera::get_zoom()
{
    return this->zoom;
}
