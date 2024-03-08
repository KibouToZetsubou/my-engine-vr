#include "cube.hpp"
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <GL/freeglut.h>

#include "common.hpp"
#include "mesh.hpp"

/**
 * Renders the cube.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param world_matrix The world matrix to use to render this object.
 */
void LIB_API Cube::render(const glm::mat4 world_matrix) const
{
    Mesh::render(world_matrix);

    glutSolidCube(1.0);
}
