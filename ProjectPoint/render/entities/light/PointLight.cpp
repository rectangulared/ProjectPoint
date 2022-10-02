#include "PointLight.h"

PointLight::PointLight() : Light(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)), position(glm::vec3(0.0f, 0.0f, 0.0f)), constant(1.0f), linear(0.09f), quadratic(0.032f) {};

PointLight::PointLight(const glm::vec3& _position, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, const GLfloat& _constant, const GLfloat& _linear, const GLfloat& _quadratic) :
	Light(_ambient, _diffuse, _specular), position(_position), constant(_constant), linear(_linear), quadratic(_quadratic) {};

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