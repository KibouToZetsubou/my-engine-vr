#include "sphere.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "common.hpp"
#include "mesh.hpp"

/**
 * Renders the sphere.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param world_matrix The world matrix to use to render this object.
 */
void LIB_API Sphere::render(const glm::mat4 world_matrix) const
{
    Mesh::render(world_matrix);

    glutSolidSphere(1.0, 16, 16);
}
