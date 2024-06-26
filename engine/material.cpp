#include "material.hpp"

/**
 * Creates a new instance of Material with the following default parameters:
 *
 * Emission color: 0.0f, 0.0f, 0.0f (Black / No emission)
 *
 * Ambient color: 0.75f, 0.75f, 0.75f (Light gray)
 *
 * Diffuse color: 0.75f, 0.75f, 0.75f (Light gray)
 *
 * Specular color: 0.75f, 0.75f, 0.75f (Light gray)
 *
 * Shininess: 64.0f
 *
 * Texture: No texture.
 */
LIB_API Material::Material()
{
    this->set_emission_color(glm::vec3(0.0f, 0.0f, 0.0f));
    this->set_ambient_color(glm::vec3(0.75f, 0.75f, 0.75f));
    this->set_diffuse_color(glm::vec3(0.75f, 0.75f, 0.75f));
    this->set_specular_color(glm::vec3(0.75f, 0.75f, 0.75f));
    this->set_shininess(64.0f);
    this->set_texture(nullptr);
}

/**
 * Renders the material.
 *
 * This function is called automatically by MyEngine and there's no need to manually call this function.
 *
 * @param view_matrix The world matrix to use to render this object.
 */
void LIB_API Material::render(const glm::mat4 view_matrix) const
{
    if (this->texture != nullptr)
    {
        this->texture->render(view_matrix);
    }
}

/**
 * Changes the emission color property of this material.
 *
 * @param new_color The new emission color of the material.
 */
void LIB_API Material::set_emission_color(const glm::vec3 new_color)
{
    this->emission_color = new_color;
}

/**
 * Used to retrieve the emission color of the material
 *
 * @return The emission color of the material
 */
glm::vec3 LIB_API Material::get_emission_color() const
{
    return this->emission_color;
}

/**
 * Changes the ambient color property of this material.
 *
 * @param new_color The new ambient color of the material.
 */
void LIB_API Material::set_ambient_color(const glm::vec3 new_color)
{
    this->ambient_color = new_color;
}

/**
 * Used to retrieve the ambient color of the material
 *
 * @return The ambient color of the material
 */
glm::vec3 LIB_API Material::get_ambient_color() const
{
    return this->ambient_color;
}

/**
 * Changes the diffuse color property of this material.
 *
 * @param new_color The new diffuse color of the material.
 */
void LIB_API Material::set_diffuse_color(const glm::vec3 new_color)
{
    this->diffuse_color = new_color;
}

/**
 * Used to retrieve the diffuse color of the material
 *
 * @return The diffuse color of the material
 */
glm::vec3 LIB_API Material::get_diffuse_color() const
{
    return this->diffuse_color;
}

/**
 * Changes the specular color property of this material.
 *
 * @param new_color The new specular color of the material.
 */
void LIB_API Material::set_specular_color(const glm::vec3 new_color)
{
    this->specular_color = new_color;
}

/**
 * Used to retrieve the specular color of the material
 *
 * @return The specular color of the material
 */
glm::vec3 LIB_API Material::get_specular_color() const
{
    return this->specular_color;
}

/**
 * Changes the shininess property of this material.
 *
 * @param new_shininess The new shininess of the material.
 */
void LIB_API Material::set_shininess(const float new_shininess)
{
    this->shininess = new_shininess;
}

/**
 * Used to retrieve the shininess of the material
 *
 * @return The shininess color of the material
 */
float LIB_API Material::get_shininess() const
{
    return this->shininess;
}

/**
 * Sets the texture of this material. If new_texture is nullptr, no texture is applied.
 *
 * @param new_texture The new texture.
 */
void LIB_API Material::set_texture(const std::shared_ptr<Texture> new_texture)
{
    this->texture = new_texture;
}

/**
 * Used to retrieve the texture of the material
 *
 * @return The texture of the material
 */
std::shared_ptr<Texture> LIB_API Material::get_texture() const
{
    return this->texture;
}
