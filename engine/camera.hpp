#pragma once

#include <glm/glm.hpp>

#include "common.hpp"
#include "node.hpp"

/**
 * This class works as a base to be inherited from to implement cameras into the scene.
 *
 * A camera renders what it has in front to the screen.
 */
class LIB_API Camera : public Node
{
public:
    Camera();

    int get_priority() const override;

    virtual glm::mat4 get_projection_matrix(const unsigned int window_width, const unsigned int window_height) const = 0;

    void set_fov(const float new_fov);
    void set_clipping(const float new_near_clipping, const float new_far_clipping);
    void set_active(const bool new_is_active);

    float get_near_clipping_plane() const;
    float get_far_clipping_plane() const;
protected:
    float fov;
    float near_clipping;
    float far_clipping;
    bool is_active;
};
