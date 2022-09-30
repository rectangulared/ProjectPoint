#include "SpotLight.h"

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