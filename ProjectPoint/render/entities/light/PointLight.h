#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Light.h"
#include <render/entities/object/model/texture/Cubemap.h>

class PointLight : public Light
{
public:
	GLfloat _constant;
	GLfloat _linear;
	GLfloat _quadratic;
	glm::vec3 _position;

	Cubemap* _shadowCubeMap;

	PointLight();
	PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic);
	PointLight(const bool& isShadowCaster, const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic, Cubemap* shadowCubemap);
};