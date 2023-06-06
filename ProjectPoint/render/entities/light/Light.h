#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	bool _isShadowCaster;
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;

	Light(const bool& isShadowCaster, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) :
		_isShadowCaster(isShadowCaster), 
		_ambient(ambient), 
		_diffuse(diffuse), 
		_specular(specular) 
	{};
};