#include "PointLight.h"

PointLight::PointLight() : Light(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)), position(glm::vec3(0.0f, 0.0f, 0.0f)), constant(1.0f), linear(0.09f), quadratic(0.032f) {};

PointLight::PointLight(const glm::vec3& _position, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, const GLfloat& _constant, const GLfloat& _linear, const GLfloat& _quadratic) :
	Light(_ambient, _diffuse, _specular), position(_position), constant(_constant), linear(_linear), quadratic(_quadratic) {};