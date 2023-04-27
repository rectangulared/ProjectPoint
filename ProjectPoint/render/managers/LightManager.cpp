#include "LightManager.h"

LightManager::LightManager() :
	_isDirectionalLightActive(true),
	_directionalLight(DirectionalLight())
{}

LightManager::LightManager(const DirectionalLight& directionalLight, const std::vector<PointLight>& pointLights, std::vector<SpotLight> spotLights) :
	_isDirectionalLightActive(true),
	_directionalLight(directionalLight),
	_pointLights(pointLights),
	_spotLights(spotLights)
{}

void LightManager::addPointLight(const PointLight& pointLight)
{
	_pointLights.push_back(pointLight);
}

void LightManager::addSpotLight(const SpotLight& spotLight)
{
	_spotLights.push_back(spotLight);
}

void LightManager::removePointLight(const GLuint& index)
{
	_pointLights.erase(_pointLights.begin() + index);
}

void LightManager::removeSpotLight(const GLuint& index)
{
	_spotLights.erase(_spotLights.begin() + index);
}

void LightManager::drawLights(UBO& uboDirectionalLight, UBO& uboPointLights, UBO& uboSpotLights)
{
	GLuint offset = 0;
	uboDirectionalLight.bind();
	uboDirectionalLight.updateSubsetData(&_isDirectionalLightActive, sizeof(GLuint), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(_directionalLight.direction), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(_directionalLight.position), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(_directionalLight.ambient), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(_directionalLight.diffuse), sizeof(glm::vec3), offset);
	offset += 16;
	uboDirectionalLight.updateSubsetData(glm::value_ptr(_directionalLight.specular), sizeof(glm::vec3), offset);
	//offset += 16;
	uboDirectionalLight.unbind();

	if (!_pointLights.empty())
	{
		offset = 0;
		GLuint pointLightsCount = _pointLights.size();
		uboPointLights.bind();
		uboPointLights.updateSubsetData(&pointLightsCount, sizeof(GLuint), offset);
		offset += 16;
		for (size_t i = 0; i < pointLightsCount; i++)
		{
			uboPointLights.updateSubsetData(glm::value_ptr(_pointLights[i].position), sizeof(glm::vec3), offset);
			offset += 12;
			uboPointLights.updateSubsetData(&_pointLights[i].constant, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&_pointLights[i].linear, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&_pointLights[i].quadratic, sizeof(GLfloat), offset);
			offset += 12;
			uboPointLights.updateSubsetData(glm::value_ptr(_pointLights[i].ambient), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(_pointLights[i].diffuse), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(_pointLights[i].specular), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
		}
		uboPointLights.unbind();
	}

	if (!_spotLights.empty())
	{
		offset = 0;
		GLuint spotLightsCount = _spotLights.size();
		uboSpotLights.bind();
		uboSpotLights.updateSubsetData(&spotLightsCount, sizeof(GLuint), offset);
		offset += 16;
		for (size_t i = 0; i < spotLightsCount; i++)
		{
			uboPointLights.updateSubsetData(glm::value_ptr(_spotLights[i].direction), sizeof(glm::vec3), offset);
			offset += 12;
			uboPointLights.updateSubsetData(&_spotLights[i].constant, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&_spotLights[i].linear, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&_spotLights[i].quadratic, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&_spotLights[i].cutOff, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(&_spotLights[i].outerCutOff, sizeof(GLfloat), offset);
			offset += 4;
			uboPointLights.updateSubsetData(glm::value_ptr(_spotLights[i].position), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(_spotLights[i].ambient), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(_spotLights[i].diffuse), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
			uboPointLights.updateSubsetData(glm::value_ptr(_spotLights[i].specular), sizeof(glm::vec3), offset);
			offset += sizeof(glm::vec4);
		}
		uboSpotLights.unbind();
	}
}