#pragma once

#include <glm/glm.hpp>

#include "Light.h"

class DirectionalLight : public Light
{
public:
	glm::vec3 direction;
	glm::vec3 position;

	DirectionalLight();

	DirectionalLight(const glm::vec3& _direction, const glm::vec3 _position, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular);
};