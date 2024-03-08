#include <assert.h>
#include <cmath>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include "camera.hpp"
#include "light.hpp"
#include "my_engine.hpp"
#include "node.hpp"
#include "object.hpp"
#include "ovo_parser.hpp"

int main()
{
	// Camera
	std::cout << "Testing Camera ..." << std::endl;
	const std::shared_ptr<Camera> camera = std::make_shared<Camera>();

	std::cout << "[Camera] Camera priority should be 200" << std::endl;
	assert(camera->get_priority() == 200);


	// Light
	std::cout << "Testing Light ..." << std::endl;
	const std::shared_ptr<Light> light = std::make_shared<Light>();

	std::cout << "[Light] Light priority should be 100" << std::endl;
	assert(light->get_priority() == 100);


	// Node
	std::cout << "Testing Node ..." << std::endl;
	std::shared_ptr<Node> node = std::make_shared<Node>();

	std::cout << "[Node] Testing node position" << std::endl;
	assert(node->get_position() == glm::vec3(0.0f, 0.0f, 0.0f));
	node->set_position(glm::vec3(1.0f, 2.0f, 3.0f));
	assert(node->get_position() == glm::vec3(1.0f, 2.0f, 3.0f));

	std::cout << "[Node] Testing node rotation" << std::endl;
	assert(node->get_rotation() == glm::vec3(0.0f, 0.0f, 0.0f));
	node->set_rotation(glm::vec3(1.0f, 2.0f, 3.0f));
	assert(node->get_rotation() == glm::vec3(1.0f, 2.0f, 3.0f));

	std::cout << "[Node] Testing node scale" << std::endl;
	assert(node->get_scale() == glm::vec3(1.0f, 1.0f, 1.0f));
	node->set_scale(glm::vec3(1.0f, 2.0f, 3.0f));
	assert(node->get_scale() == glm::vec3(1.0f, 2.0f, 3.0f));

	/*std::cout << "[Node] Testing node local matrix" << std::endl;
	const glm::mat4 expected_matrix(0.9980f, -0.0516f, -0.0358f, 0.0000f, 0.1046f, 1.9969f, 0.0348f, 0.0000f, 0.1045f, -0.0578f, 2.9976f, 0.0000f, 1.0000f, 2.0000f, 3.0000f, 1.0000f);
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			const float difference = std::abs(expected_matrix[row][col] - node->get_local_matrix()[row][col]);
			assert(difference < 0.001f);
		}
	}*/


	// Object
	std::cout << "Testing Object ..." << std::endl;
	// We can't actually create an instance of Object, so we just use Node.
	std::shared_ptr<Object> object = std::make_shared<Node>();

	std::cout << "[Object] Testing object name" << std::endl;
	object->set_name("foo");
	assert(object->get_name() == "foo");

	std::cout << "[Object] Testing object ID" << std::endl;
	assert(object->get_id() == 4);

	{
		std::cout << "[Object] Testing object children" << std::endl;
		std::shared_ptr<Object> child1 = std::make_shared<Node>();
		child1->set_name("Child 1");
		object->add_child(child1);

		std::shared_ptr<Object> child2 = std::make_shared<Node>();
		child2->set_name("Child 2");
		object->add_child(child2);

		const auto children = object->get_children();
		assert(children.size() == 2);
		assert(children[0]->get_name() == "Child 1");
		assert(children[1]->get_name() == "Child 2");
	}

	std::cout << "[Object] Testing object local matrix" << std::endl;
	assert(object->get_local_matrix() == glm::mat4(1.0f));

	std::cout << "[Object] Testing object priority" << std::endl;
	assert(object->get_priority() == 0);


	std::cout << "Testing OVOParser ..." << std::endl;
	const std::shared_ptr<Object> root = OVOParser::from_file("./test.ovo");

	{
		std::cout << "[OVOParser] Testing parser scene root" << std::endl;
		assert(root->get_name() == "Scene Root");
		assert(root->get_children().size() == 1);

		const std::shared_ptr<Object> scene_root = root->get_children()[0];
		assert(scene_root->get_name() == "[root]");
		assert(scene_root->get_children().size() == 1);

		const std::shared_ptr<Object> box001 = scene_root->get_children()[0];
		assert(box001->get_name() == "Box001");
		assert(box001->get_children().size() == 0);
	}

	return 0;
}
