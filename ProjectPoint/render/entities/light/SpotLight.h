#pragma once

#include <glm/glm.hpp>

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();

	SpotLight(const glm::vec3& _position, const glm::vec3& _direction, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, const GLfloat& _constant, const GLfloat& _linear, const GLfloat& _quadratic, const GLfloat& _cutOff, const GLfloat& _outerCutOff);

	GLfloat getCutOff() const;
	GLfloat getOuterCutOff() const;
	glm::vec3 getDirection() const;

	void setCutOff(const GLfloat& cutOff);
	void setOuterCutOff(const GLfloat& outerCutOff);
	void setDirection(const glm::vec3& direction);

private:
	GLfloat cutOff;
	GLfloat outerCutOff;
	glm::vec3 direction;
};