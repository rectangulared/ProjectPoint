#include "PointLight.h"

PointLight::PointLight() : 
	Light(false, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)), 
	_position(glm::vec3(0.0f, 0.0f, 0.0f)), 
	_constant(1.0f), 
	_linear(0.09f), 
	_quadratic(0.032f),
	_shadowCubeMap(nullptr)
{};

PointLight::PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic) :
	Light(false, ambient, diffuse, specular), 
	_position(position), 
	_constant(constant),
	_linear(linear),
	_quadratic(quadratic),
	_shadowCubeMap(nullptr)
{};

PointLight::PointLight(const bool& isShadowCaster, const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic, Cubemap* shadowCubemap) :
	Light(isShadowCaster, ambient, diffuse, specular),
	_position(position),
	_constant(constant),
	_linear(linear),
	_quadratic(quadratic),
	_shadowCubeMap(shadowCubemap)
{};