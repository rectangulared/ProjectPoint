#pragma once

#include <vector>

#include "glm/glm.hpp"

enum RenderType
{
	OBJECT,
	INSTANCED_OBJECT
};

class Renderable
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> scales;
	std::vector<glm::quat> rotations;
	std::vector<glm::mat4> modelMatrices;
};