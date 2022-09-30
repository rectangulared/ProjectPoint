#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f)), direction(glm::vec3(-0.2f, -1.0f, -0.3f)) {};

glm::vec3 DirectionalLight::getDirection() const
{
	return direction;
}

void DirectionalLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}