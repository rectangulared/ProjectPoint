#include "SpotLight.h"

SpotLight::SpotLight() : 
	PointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f), 
	_cutOff(glm::cos(glm::radians(12.5f))), 
	_outerCutOff(glm::cos(glm::radians(17.5f))), 
	_direction(glm::vec3(0.0f, 0.0f, 0.0f)),
	_shadowMap(nullptr)
{};

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic, const GLfloat& cutOff, const GLfloat& outerCutOff) :
	PointLight(position, ambient, diffuse, specular, constant, linear, quadratic), 
	_cutOff(glm::cos(glm::radians(cutOff))), 
	_outerCutOff(glm::cos(glm::radians(outerCutOff))), 
	_direction(direction),
	_shadowMap(nullptr)
{};

SpotLight::SpotLight(const bool& isCastingShadows, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic, const GLfloat& cutOff, const GLfloat& outerCutOff, Texture* shadowMap) :
	PointLight(isCastingShadows, position, ambient, diffuse, specular, constant, linear, quadratic, nullptr),
	_cutOff(glm::cos(glm::radians(cutOff))),
	_outerCutOff(glm::cos(glm::radians(outerCutOff))),
	_direction(direction),
	_shadowMap(shadowMap)
{};


void SpotLight::setCutOffInDegrees(const GLfloat& degrees)
{
	_cutOff = glm::cos(glm::radians(degrees));
}

void SpotLight::setOuterCutOffInDegrees(const GLfloat& degrees)
{
	_outerCutOff = glm::cos(glm::radians(degrees));
}

GLfloat SpotLight::getCutOffInDegrees()
{
	return glm::degrees(glm::acos(_cutOff));
}
GLfloat SpotLight::getOuterCutOffInDegrees()
{
	return glm::degrees(glm::acos(_outerCutOff));
}