#pragma once

#include <glm/glm.hpp>

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	GLfloat cutOff;
	GLfloat outerCutOff;
	glm::vec3 direction;

	SpotLight();

	SpotLight(const glm::vec3& _position, const glm::vec3& _direction, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, const GLfloat& _constant, const GLfloat& _linear, const GLfloat& _quadratic, const GLfloat& _cutOff, const GLfloat& _outerCutOff);

	void setCutOffInDegrees(const GLfloat& degrees);
	void setOuterCutOffInDegrees(const GLfloat& degrees);

	GLfloat getCutOffInDegrees();
	GLfloat getOuterCutOffInDegrees();
};