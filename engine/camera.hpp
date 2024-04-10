#pragma once

#include <glm/glm.hpp>

#include "common.hpp"
#include "node.hpp"

// TODO: Add a function to return the projection matrix

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
    void set_window_size(const int new_width, const int new_height);
    void set_fov(const float new_fov);
    void set_clipping(const float new_near_clipping, const float new_far_clipping);
    void set_active(const bool new_is_active);
protected:
    float fov;
    float near_clipping;
    float far_clipping;
    int window_width;
    int window_height;
    bool is_active;
};
