#pragma once

#include "common.hpp"
#include "light.hpp"

/**
 * A point light that can be placed inside a scene.
 *
 * A point light emits light in any direction with a limited range.
 */
class LIB_API PointLight : public Light
{
public:
    PointLight();

    void render(const glm::mat4 view_matrix) const override;

    void set_radius(const float new_radius);
private:
    float radius;
};
