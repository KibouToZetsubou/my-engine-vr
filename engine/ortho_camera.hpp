#pragma once

#include <glm/glm.hpp>

#include "common.hpp"
#include "camera.hpp"

/**
 * A type of camera that renders the scene without any perspective.
 */
class LIB_API OrthoCamera : public Camera
{
public:
    void render(const glm::mat4 view_matrix) const override;
    glm::mat4 get_projection_matrix(const unsigned int window_width, const unsigned int window_height) const override;

    void set_zoom(float new_zoom);
    float get_zoom();
private:
    float zoom;
};
