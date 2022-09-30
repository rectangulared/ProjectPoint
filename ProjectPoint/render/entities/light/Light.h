#pragma once

#include <glm/glm.hpp>

class Light
{
public:

	Light(const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular) : ambient(_ambient), diffuse(_diffuse), specular(_specular) {};

	virtual glm::vec3& getAmbient()
	{
		return ambient;
	}

	virtual glm::vec3& getDiffuse()
	{
		return diffuse;
	}

	virtual glm::vec3& getSpecular()
	{
		return specular;
	}

	virtual void setAmbient(const glm::vec3& ambient)
	{
		this->ambient = ambient;
	}

	virtual void setDiffuse(const glm::vec3& diffuse)
	{
		this->diffuse = diffuse;
	}

	virtual void setSpecular(const glm::vec3& specular)
	{
		this->specular = specular;
	}

	virtual ~Light() = default;

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};