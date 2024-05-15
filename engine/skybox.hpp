#pragma once

#include <memory>
#include <vector>

#include "common.hpp"
#include "node.hpp"
#include "material.hpp"

class LIB_API Skybox : public Node
{
public:
	Skybox(const std::vector<std::string> textures);
	~Skybox();

	void render(const glm::mat4 view_matrix) const override;
private:
	std::shared_ptr<Material> material;

	unsigned int vbo_vertices;
	unsigned int vbo_faces;
	unsigned int cubemap_id;
	unsigned int vao_id;

	static unsigned short faces[];
};
