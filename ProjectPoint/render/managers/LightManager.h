#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

#include "render/ShaderProgram.h"
#include "render/entities/light/DirectionalLight.h"
#include "render/entities/light/PointLight.h"
#include "render/entities/light/SpotLight.h"
#include <render/entities/buffers/UBO/UBO.h>

//TODO: Add possibility to inherit position from another object and offsets
//TODO: Add light icons sprites at every light position
class LightManager
{
public:
	GLboolean _isDirectionalLightActive;

	DirectionalLight _directionalLight;
	std::vector<PointLight> _pointLights;
	std::vector<SpotLight> _spotLights;

	LightManager();
	LightManager(const DirectionalLight& directionalLight, const std::vector<PointLight>& pointLights, std::vector<SpotLight> spotLights);

	void addPointLight(const PointLight& pointLight);
	void addSpotLight(const SpotLight& spotLight);
	void removePointLight(const GLuint& index);
	void removeSpotLight(const GLuint& index);

	void drawLights(UBO& uboDirectionalLight, UBO& uboPointLights, UBO& uboSpotLights);
};
