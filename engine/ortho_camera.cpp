#include "ortho_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

/**
 * Renders the camera.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param view_matrix The world matrix to use to render this object.
 */
void LIB_API OrthoCamera::render(const glm::mat4 view_matrix) const
{
    Node::render(view_matrix);
}

glm::mat4 LIB_API OrthoCamera::get_projection_matrix(const unsigned int window_width, const unsigned int window_height) const
{
    const float width = static_cast<float>(window_width);
    const float height = static_cast<float>(window_height);

    const float max = std::max(width, height);

    const float w = (width / max) * this->zoom;
    const float h = (height / max) * this->zoom;

    return glm::ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f, this->near_clipping, this->far_clipping);
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
        new_zoom = 0.0f; // To prevent funny loop around
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
