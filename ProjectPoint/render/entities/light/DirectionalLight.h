#pragma once

#include <glm/glm.hpp>

#include "Light.h"
#include <render/entities/object/model/texture/Texture.h>

class DirectionalLight : public Light
{
public:
	glm::vec3 _direction;
	glm::vec3 _position;

	Texture* _shadowMap;

	DirectionalLight();

	DirectionalLight(const glm::vec3& direction, const glm::vec3 position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

	DirectionalLight(const GLboolean& isShadowCaster, const glm::vec3& direction, const glm::vec3 position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, Texture* shadowMap);

	~DirectionalLight()
	{
		delete _shadowMap;
		_shadowMap = nullptr;
	}
};