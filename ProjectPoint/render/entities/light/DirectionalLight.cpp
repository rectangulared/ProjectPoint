#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f)), direction(glm::vec3(-0.2f, -1.0f, -0.3f)), position(glm::vec3(2.0f, 2.0f, 2.0f)) {};

DirectionalLight::DirectionalLight(const glm::vec3& _direction, const glm::vec3 _position, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular) : Light(_ambient, _diffuse, _specular), direction(_direction), position(_position) {};