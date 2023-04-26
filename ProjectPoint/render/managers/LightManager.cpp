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

void LightManager::drawLights(UBO& uboDirectionalLight, UBO& uboPointLights, UBO& uboSpotLights)
{
	GLuint offset = 0;
	uboDirectionalLight.bind();
	uboDirectionalLight.updateSubsetData(&isDirectionalLightActive, sizeof(GLuint), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(directionalLight.direction), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(directionalLight.position), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(directionalLight.ambient), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(directionalLight.diffuse), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(directionalLight.specular), sizeof(glm::vec3), offset);
	//offset += 16;
	uboDirectionalLight.unbind();

	if (!pointLights.empty())
	{
		offset = 0;
		GLuint pointLightsCount = pointLights.size();
		uboPointLights.bind();
		uboPointLights.updateSubsetData(&pointLightsCount, sizeof(GLuint), offset);
		offset += 16;
		for (size_t i = 0; i < pointLightsCount; i++)
		{
			uboPointLights.updateSubsetData(glm::value_ptr(pointLights[i].position), sizeof(glm::vec3), offset);
			offset += 12;
			uboPointLights.updateSubsetData(&pointLights[i].constant, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&pointLights[i].linear, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&pointLights[i].quadratic, sizeof(GLfloat), offset);
			offset += 12;
			uboPointLights.updateSubsetData(glm::value_ptr(pointLights[i].ambient), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(pointLights[i].diffuse), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(pointLights[i].specular), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
		}
		uboPointLights.unbind();
	}

	if (!spotLights.empty())
	{
		offset = 0;
		GLuint spotLightsCount = spotLights.size();
		uboSpotLights.bind();
		uboSpotLights.updateSubsetData(&spotLightsCount, sizeof(GLuint), offset);
		offset += 16;
		for (size_t i = 0; i < spotLightsCount; i++)
		{
			uboPointLights.updateSubsetData(glm::value_ptr(spotLights[i].direction), sizeof(glm::vec3), offset);
			offset += 12;
			uboPointLights.updateSubsetData(&spotLights[i].constant, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&spotLights[i].linear, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&spotLights[i].quadratic, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&spotLights[i].cutOff, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&spotLights[i].outerCutOff, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(glm::value_ptr(spotLights[i].position), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(spotLights[i].ambient), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(spotLights[i].diffuse), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(spotLights[i].specular), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
		}
		uboSpotLights.unbind();
	}
}