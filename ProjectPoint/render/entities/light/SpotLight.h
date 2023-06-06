#pragma once

#include <glm/glm.hpp>

#include "PointLight.h"
#include <render/entities/object/model/texture/Texture.h>

class SpotLight : public PointLight
{
public:
	GLfloat _cutOff;
	GLfloat _outerCutOff;
	glm::vec3 _direction;

	Texture* _shadowMap;

	SpotLight();

	SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic, const GLfloat& cutOff, const GLfloat& outerCutOff);

	SpotLight(const bool& isCastingShadows, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLfloat& constant, const GLfloat& linear, const GLfloat& quadratic, const GLfloat& cutOff, const GLfloat& outerCutOff, Texture* shadowMap);

	void setCutOffInDegrees(const GLfloat& degrees);
	void setOuterCutOffInDegrees(const GLfloat& degrees);

	GLfloat getCutOffInDegrees();
	GLfloat getOuterCutOffInDegrees();
};