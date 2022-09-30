#pragma once

#include <glm/glm.hpp>

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();

	DirectionalLight(const glm::vec3& _direction, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular) : Light(_ambient, _diffuse, _specular), direction(_direction) {};

	glm::vec3 getDirection() const;

	void setDirection(const glm::vec3& direction);

private:
	glm::vec3 direction;
};