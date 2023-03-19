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

void LightManager::drawLights(ShaderProgram& shaderProgram, const GLuint& ubo)
{
	shaderProgram.use();
	GLuint offset = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(GLuint), &isDirectionalLightActive);
	offset += 4;
	GLint pointLightsCount = pointLights.size();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(GLuint), &pointLightsCount);
	offset += 4;
	GLint spotLightsCount = spotLights.size();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(GLuint), &spotLightsCount);
	offset += 8;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(directionalLight.direction));
	offset += sizeof(glm::vec4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(directionalLight.ambient));
	offset += sizeof(glm::vec4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(directionalLight.diffuse));
	offset += sizeof(glm::vec4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(directionalLight.specular));
	offset += sizeof(glm::vec4);

	if (!pointLights.empty())
	{
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(pointLights[i].position));
			offset += sizeof(glm::vec4);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(GLfloat), &pointLights[i].constant);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(GLfloat), &pointLights[i].linear);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(GLfloat), &pointLights[i].quadratic);
			offset += 8;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(pointLights[i].ambient));
			offset += sizeof(glm::vec4);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(pointLights[i].diffuse));
			offset += sizeof(glm::vec4);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(pointLights[i].specular));
			offset += sizeof(glm::vec4);

		}
	}

	if (!spotLights.empty())
	{
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(glm::vec3), glm::value_ptr(spotLights[i].direction));
			offset += 12;
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(GLfloat), &spotLights[i].constant);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(GLfloat), &spotLights[i].linear);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(GLfloat), &spotLights[i].quadratic);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(GLfloat), &spotLights[i].cutOff);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(GLfloat), &spotLights[i].outerCutOff);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(glm::vec3), glm::value_ptr(spotLights[i].position));
			offset += sizeof(glm::vec4);
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(glm::vec3), glm::value_ptr(spotLights[i].ambient));
			offset += sizeof(glm::vec4);
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(glm::vec3), glm::value_ptr(spotLights[i].diffuse));
			offset += sizeof(glm::vec4);
			glBufferSubData(GL_UNIFORM_BUFFER, offset + 2640, sizeof(glm::vec3), glm::value_ptr(spotLights[i].specular));
			offset += sizeof(glm::vec4);
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}