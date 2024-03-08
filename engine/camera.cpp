#include "camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL

#include "common.hpp"

/**
 * Creates a new instance of Camera with the following default parameters:
 *
 * FOV: 90.0f
 *
 * Near clipping: 0.01f
 *
 * Far clipping: 1000.0f
 */
LIB_API Camera::Camera()
{
    this->set_fov(90.0f);
    this->set_clipping(0.01f, 1000.0f);
    //this->set_window_size(1, 1);
    this->is_active = false;
    this->set_window_size(0, 0);
}

/**
 * Returns the priority of this object.
 *
 * Cameras have a priority of 200.
 *
 * @return The priority of this object.
 */
int LIB_API Camera::get_priority() const
{
    return 200;
}

/**
 * Sets the size of the camera frame.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param new_width The width of the frame in pixels.
 * @param new_height The height of the frame in pixels.
 */
void LIB_API Camera::set_window_size(const int new_width, const int new_height)
{
    this->window_width = new_width;
    this->window_height = new_height;
}

/**
 * Sets the camera Field of View (FOV).
 *
 * @param new_fov The new value of the FOV.
 */
void LIB_API Camera::set_fov(const float new_fov)
{
    this->fov = new_fov;
}

/**
 * Sets the camera far and near clipping planes.
 *
 * @param new_near_clipping The near clipping plane of the camera.
 * @param new_far_clipping The far clipping plane of the camera.
 */
void LIB_API Camera::set_clipping(const float new_near_clipping, const float new_far_clipping)
{
    this->near_clipping = new_near_clipping;
    this->far_clipping = new_far_clipping;
}

/**
 * Sets the camera activation flag.
 *
 * @param new_is_active The new value of the activation flag.
 */
void LIB_API Camera::set_active(const bool new_is_active)
{
    this->is_active = new_is_active;
}
