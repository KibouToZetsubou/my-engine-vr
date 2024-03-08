#pragma once

#include "common.hpp"
#include "mesh.hpp"

/**
 * A simple 3D sphere that can be placed inside a scene. It is designed for development and debug purposes.
 */
class LIB_API Sphere : public Mesh
{
public:
    void render(const glm::mat4 world_matrix) const override;
};
