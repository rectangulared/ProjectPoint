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

void LightManager::changePointLight(const GLuint& index, const PointLight& pointLight)
{
	pointLights[index] = pointLight;
}

void LightManager::changeSpotLight(const GLuint& index, const SpotLight& spotLight)
{
	spotLights[index] = spotLight;
}

void LightManager::changeDirectionalLight(const DirectionalLight& directionalLight)
{
	this->directionalLight = directionalLight;
}

void LightManager::directionalLightSwitch(const GLboolean isDirectionalLightActive)
{
	this->isDirectionalLightActive = isDirectionalLightActive;
}

void LightManager::setupLights(ShaderProgram& shaderProgram)
{
	shaderProgram.use();
	if (isDirectionalLightActive)
	{
		shaderProgram.setBool("isDirLight", isDirectionalLightActive);
		shaderProgram.setVec3f("dirLight.direction", directionalLight.getDirection());
		shaderProgram.setVec3f("dirLight.ambient", directionalLight.getAmbient());
		shaderProgram.setVec3f("dirLight.diffuse", directionalLight.getDiffuse());
		shaderProgram.setVec3f("dirLight.specular", directionalLight.getSpecular());
	}
	if (!pointLights.empty())
	{
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].position", pointLights[i].getPosition());
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].getAmbient());
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].getDiffuse());
			shaderProgram.setVec3f("pointLights[" + std::to_string(i) + "].specular", pointLights[i].getSpecular());
			shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].getConstant());
			shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].getLinear());
			shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].getQuadratic());
		}
	}
	shaderProgram.setUInt("activePointLights", pointLights.size());

	if (!spotLights.empty())
	{
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].position", spotLights[i].getPosition());
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].direction", spotLights[i].getDirection());
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].ambient", spotLights[i].getAmbient());
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].diffuse", spotLights[i].getDiffuse());
			shaderProgram.setVec3f("spotLights[" + std::to_string(i) + "].specular", spotLights[i].getSpecular());
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].constant", spotLights[i].getConstant());
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].linear", spotLights[i].getLinear());
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].quadratic", spotLights[i].getQuadratic());
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].cutOff", spotLights[i].getCutOff());
			shaderProgram.setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", spotLights[i].getOuterCutOff());
		}
	}
	shaderProgram.setUInt("activeSpotLights", spotLights.size());

}

GLuint LightManager::getActivePointLights()
{
	return static_cast<GLuint>(pointLights.size());
}

GLuint LightManager::getActiveSpotLights()
{
	return static_cast<GLuint>(spotLights.size());
}