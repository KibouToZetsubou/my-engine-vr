#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "common.hpp"
#include "camera.hpp"

/**
 * A type of camera that renders the scene with perspective.
 */
class LIB_API PerspectiveCamera : public Camera
{
public:
    void render(const glm::mat4 world_matrix) const override;
};
