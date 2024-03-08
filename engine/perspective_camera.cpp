#include "perspective_camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
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
void LIB_API PerspectiveCamera::render(const glm::mat4 world_matrix) const
{
    if (!this->is_active)
    {
        return;
    }

    Node::render(world_matrix);

    const float aspect_ratio = static_cast<float>(this->window_width) / static_cast<float>(this->window_height);

    const glm::mat4 perspective_matrix = glm::perspective(glm::radians(this->fov), aspect_ratio, this->near_clipping, this->far_clipping);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(perspective_matrix));
}
