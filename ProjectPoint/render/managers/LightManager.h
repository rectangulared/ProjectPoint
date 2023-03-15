#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

#include "render/ShaderProgram.h"
#include "render/entities/light/DirectionalLight.h"
#include "render/entities/light/PointLight.h"
#include "render/entities/light/SpotLight.h"

//TODO: Add possibility to inherit position from another object and offsets
//TODO: Add light icons sprites at every light position
class LightManager
{
public:
	GLboolean isDirectionalLightActive;

	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	LightManager();
	LightManager(const DirectionalLight& _directionalLight, const std::vector<PointLight>& _pointLights, std::vector<SpotLight> _spotLights);

	void addPointLight(const PointLight& pointLight);
	void addSpotLight(const SpotLight& spotLight);
	void removePointLight(const GLuint& index);
	void removeSpotLight(const GLuint& index);

	void drawLights(ShaderProgram& shaderProgram, const GLuint& uboRangeIndex);
};
