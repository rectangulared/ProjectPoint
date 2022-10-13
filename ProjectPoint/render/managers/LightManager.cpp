#include "LightManager.h"

LightManager::LightManager() : directionalLight(DirectionalLight()) 
{
	isDirectionalLightActive = true;
};

LightManager::LightManager(const DirectionalLight& _directionalLight, const std::vector<PointLight>& _pointLights, std::vector<SpotLight> _spotLights) : directionalLight(_directionalLight), pointLights(_pointLights), spotLights(_spotLights)
{
	isDirectionalLightActive = true;
}

void LightManager::addPointLight(const PointLight& pointLight)
{
	pointLights.push_back(pointLight);
}

void LightManager::addSpotLight(const SpotLight& spotLight)
{
	spotLights.push_back(spotLight);
}

void LightManager::removePointLight(const GLuint& index)
{
	pointLights.erase(pointLights.begin() + index);
}

void LightManager::removeSpotLight(const GLuint& index)
{
	spotLights.erase(spotLights.begin() + index);
}

void LightManager::drawLights(ShaderProgram& shaderProgram)
{
	shaderProgram.use();

	shaderProgram.setBool("isDirLight", isDirectionalLightActive);
	shaderProgram.setVec3f("dirLight.direction", directionalLight.direction);
	shaderProgram.setVec3f("dirLight.ambient", directionalLight.ambient);
	shaderProgram.setVec3f("dirLight.diffuse", directionalLight.diffuse);
	shaderProgram.setVec3f("dirLight.specular", directionalLight.specular);

	if (!pointLights.empty())
	{
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].ambient);
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].diffuse);
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].specular", pointLights[i].specular);
			shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
			shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
			shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
		}
	}
	shaderProgram.setUInt("activePointLights", pointLights.size());

	if (!spotLights.empty())
	{
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].position", spotLights[i].position);
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].direction", spotLights[i].direction);
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].ambient", spotLights[i].ambient);
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].diffuse", spotLights[i].diffuse);
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].specular", spotLights[i].specular);
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].constant", spotLights[i].constant);
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].linear", spotLights[i].linear);
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].quadratic", spotLights[i].quadratic);
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].cutOff", spotLights[i].cutOff);
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", spotLights[i].outerCutOff);
		}
	}
	shaderProgram.setUInt("activeSpotLights", spotLights.size());

}