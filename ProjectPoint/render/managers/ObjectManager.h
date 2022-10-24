#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <unordered_map>
#include "../entities/object/Object.h"

class ObjectManager
{
public:
	std::vector<Object*> objects;

	ObjectManager();
	~ObjectManager();

	void addObject(Object* object);
	void removeObject(unsigned int index);
	void draw(const glm::vec3& cameraPosition, ShaderProgram& opaqueShaderProgram, ShaderProgram& transparentShaderProgram);
};