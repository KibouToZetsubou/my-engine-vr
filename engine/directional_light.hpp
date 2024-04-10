#pragma once

#include <glm/glm.hpp>

#include "common.hpp"
#include "light.hpp"

/**
 * A directional light that can be placed inside a scene.
 *
 * A directional light is a light that has infinite range and a direction. Example: the sun.
 */
class LIB_API DirectionalLight : public Light
{
public:
    DirectionalLight();

    void render(const glm::mat4 view_matrix) const override;

    void set_direction(const glm::vec3 new_direction);
private:
    glm::vec3 direction;
};
