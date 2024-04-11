#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "common.hpp"
#include "object.hpp"
#include "texture.hpp"

/**
 * A material gives colors to meshes.
 */
class LIB_API Material : public Object
{
public:
    Material();

    void render(const glm::mat4 view_matrix) const override;

    void set_emission_color(const glm::vec3 new_color);
    glm::vec3 get_emission_color() const;

    void set_ambient_color(const glm::vec3 new_color);
    glm::vec3 get_ambient_color() const;

    void set_diffuse_color(const glm::vec3 new_color);
    glm::vec3 get_diffuse_color() const;

    void set_specular_color(const glm::vec3 new_color);
    glm::vec3 get_specular_color() const;

    void set_shininess(const float new_shininess);
    float get_shininess() const;

    void set_texture(const std::shared_ptr<Texture> new_texture);
    std::shared_ptr<Texture> get_texture() const;
private:
    glm::vec3 emission_color;
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float shininess;
    std::shared_ptr<Texture> texture;
};
