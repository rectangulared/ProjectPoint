#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <unordered_map>
#include "render/entities/object/Object.h"

class ObjectManager
{
public:
	std::vector<Object*> _objects;

	ObjectManager();
	~ObjectManager();

	void addObject(Object* object);
	void draw(const glm::vec3& cameraPosition, ShaderProgram& opaqueShaderProgram, ShaderProgram& transparentShaderProgram, ShaderProgram& instancingShaderProgram);
};