#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : 
	Light(false, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f)), 
	_direction(glm::vec3(0.0f, 0.0f, 0.0f)), 
	_position(glm::vec3(0.0f, 5.0f, 0.0f)),
	_shadowMap(nullptr)
{};

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3 position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) :
	Light(false, ambient, diffuse, specular),
	_direction(direction),
	_position(position),
	_shadowMap(nullptr)
{};

DirectionalLight::DirectionalLight(const GLboolean& isShadowCaster, const glm::vec3& direction, const glm::vec3 position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, Texture* shadowMap) :
	Light(isShadowCaster, ambient, diffuse, specular),
	_direction(direction),
	_position(position),
	_shadowMap(shadowMap)
{};