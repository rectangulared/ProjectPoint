#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Light.h"

class PointLight : public Light
{
public:
	GLfloat constant = 1.0f;
	GLfloat linear = 0.09f;
	GLfloat quadratic = 0.032f;
	glm::vec3 position;

	PointLight();
	PointLight(const glm::vec3& _position, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, const GLfloat& _constant, const GLfloat& _linear, const GLfloat& _quadratic);
};