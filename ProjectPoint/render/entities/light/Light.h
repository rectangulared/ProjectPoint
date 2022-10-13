#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular) : ambient(_ambient), diffuse(_diffuse), specular(_specular) {};
};