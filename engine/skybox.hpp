#pragma once

#include <memory>
#include <vector>

#include "common.hpp"
#include "node.hpp"
#include "material.hpp"

class LIB_API Skybox : public Node {
public:
	Skybox(std::vector<std::string> cubamepNames);
	~Skybox();
	void render(const glm::mat4 view_matrix) const override;


private:
	void generateCubeMap(std::vector<std::string> cubamepNames);

	std::shared_ptr<Material> material;

	unsigned int vbo_vertices;
	unsigned int vbo_faces;
	unsigned int cubemap_id;
	unsigned int vao_id;
};
