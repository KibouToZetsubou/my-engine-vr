#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>   

#include <GL/glew.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <LeapC.h>

#include "common.hpp"

class LIB_API Leap
{
public:
	Leap();
	~Leap();

	// Init/free:
	bool init();
	bool free();

	// Polling:
	bool update();
	const LEAP_TRACKING_EVENT* getCurFrame() const;
	
	unsigned int get_vertices_size() const;

	void bind_vao();

private:
	LEAP_CONNECTION connection;
	LEAP_DEVICE_REF leapDevice;
	LEAP_TRACKING_EVENT curFrame;
	signed long long lastFrameId;

	unsigned int vao;
	unsigned int vbo;
	std::vector<glm::vec3> vertices;
};
