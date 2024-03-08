#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "common.hpp"
#include "object.hpp"

/**
 * A node represents an invisible object inside the scene with a position, rotation and scale.
 */
class LIB_API Node : public Object
{
public:
    Node();

    void set_base_matrix(const glm::mat4 new_base_matrix);

    void set_position(const glm::vec3 new_position);
    void set_rotation(const glm::vec3 new_position);
    void set_scale(const glm::vec3 new_position);

    glm::vec3 get_position() const;
    glm::vec3 get_rotation() const;
    glm::vec3 get_scale() const;

    glm::mat4 get_local_matrix() const override;

    void render(const glm::mat4 world_matrix) const override;
private:
    glm::mat4 base_matrix;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};
