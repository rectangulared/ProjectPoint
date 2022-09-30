#include "PointLight.h"

GLfloat PointLight::getConstant() const
{
	return constant;
}

GLfloat PointLight::getLinear() const
{
	return linear;
}

GLfloat PointLight::getQuadratic() const
{
	return quadratic;
}

glm::vec3 PointLight::getPosition() const
{
	return position;
}

void PointLight::setConstant(const GLfloat& constant)
{
	this->constant = constant;
}

void PointLight::setLinear(const GLfloat& linear)
{
	this->linear = linear;
}

void PointLight::setQuadratic(const GLfloat& quadratic)
{
	this->quadratic = quadratic;
}

void PointLight::setPosition(const glm::vec3& position)
{
	this->position = position;
}