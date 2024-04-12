#include "perspective_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

/**
 * Renders the camera.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param view_matrix The world matrix to use to render this object.
 */
void LIB_API PerspectiveCamera::render(const glm::mat4 view_matrix) const
{
    if (!this->is_active)
    {
        return;
    }

    Node::render(view_matrix);

    const float aspect_ratio = static_cast<float>(this->window_width) / static_cast<float>(this->window_height);

    const glm::mat4 perspective_matrix = glm::perspective(glm::radians(this->fov), aspect_ratio, this->near_clipping, this->far_clipping);
}
