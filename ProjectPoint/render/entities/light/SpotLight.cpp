#include "SpotLight.h"

SpotLight::SpotLight() : PointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f), cutOff(glm::cos(glm::radians(12.5f))), outerCutOff(glm::cos(glm::radians(17.5f))), direction(glm::vec3(0.0f, 0.0f, 0.0f)) {};

SpotLight::SpotLight(const glm::vec3& _position, const glm::vec3& _direction, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, const GLfloat& _constant, const GLfloat& _linear, const GLfloat& _quadratic, const GLfloat& _cutOff, const GLfloat& _outerCutOff) :
	PointLight(_position, _ambient, _diffuse, _specular, _constant, _linear, _quadratic), cutOff(glm::cos(glm::radians(_cutOff))), outerCutOff(glm::cos(glm::radians(_outerCutOff))), direction(_direction) {};

GLfloat SpotLight::getCutOff() const
{
	return cutOff;
}

GLfloat SpotLight::getOuterCutOff() const
{
	return outerCutOff;
}

glm::vec3 SpotLight::getDirection() const
{
	return direction;
}

void SpotLight::setCutOff(const GLfloat& cutOff)
{
	this->cutOff = cutOff;
}

void SpotLight::setOuterCutOff(const GLfloat& outerCutOff)
{
	this->outerCutOff = outerCutOff;
}

void SpotLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}