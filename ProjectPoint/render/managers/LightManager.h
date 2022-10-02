#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

#include "../ShaderProgram.h"
#include "../entities/light/DirectionalLight.h"
#include "../entities/light/PointLight.h"
#include "../entities/light/SpotLight.h"

//TODO: Add possibility to inherit position from another object and offsets
class LightManager
{
public:
	LightManager();
	LightManager(const DirectionalLight& _directionalLight, const std::vector<PointLight>& _pointLights, std::vector<SpotLight> _spotLights);

	void addPointLight(const PointLight& pointLight);
	void addSpotLight(const SpotLight& spotLight);
	void addPointLight();
	void addSpotLight();
	void removePointLight(const GLuint& index);
	void removeSpotLight(const GLuint& index);
	void changePointLight(const GLuint& index, const PointLight& pointLight);
	void changeSpotLight(const GLuint& index, const SpotLight& spotLight);
	void changeDirectionalLight(const DirectionalLight& directionalLight);
	void directionalLightSwitch();

	void drawLights(ShaderProgram& shaderProgram);

	GLuint getActivePointLights();
	GLuint getActiveSpotLights();

	DirectionalLight* getDirectionalLight();
	PointLight* getPointLightByIndex(const GLuint& index);
	SpotLight* getSpotLightByIndex(const GLuint& index);

private:
	GLboolean isDirectionalLightActive;

	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
};
