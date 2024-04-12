#pragma once

#include "common.hpp"
#include "light.hpp"

/**
 * A spot light that can be placed inside a scene.
 *
 * A spot light emits light in the shape of a cone with a specific angle and range.
 */
class LIB_API SpotLight : public Light
{
public:
    SpotLight();

    void render(const glm::mat4 view_matrix) const override;

    void set_cutoff(const float new_cutoff);
    float get_cutoff() const;

    void set_radius(const float new_radius);
    float get_radius() const;

    void set_exponent(const float new_exponent);
    float get_exponent() const;

    void set_direction(const glm::vec3 new_direction);
    glm::vec3 get_direction() const;
private:
    float cutoff;
    float radius;
    float exponent;
    glm::vec3 direction;
};
