#pragma once

#include "common.hpp"
#include "node.hpp"

/**
 * This class works as a base to be inherited from to implement lights into the scene.
 *
 * A light illuminates every object it touches.
 */
class LIB_API Light : public Node
{
public:
    Light();

    int get_priority() const override;

    void set_ambient_color(const glm::vec3 new_color);
    glm::vec3 get_ambient_color() const;

    void set_diffuse_color(const glm::vec3 new_color);
    glm::vec3 get_diffuse_color() const;

    void set_specular_color(const glm::vec3 new_color);
    glm::vec3 get_specular_color() const;
protected:
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
};
