#pragma once

#include <string>
#include <unordered_map>

#include "render/ShaderProgram.h"

class ShaderStorage
{
private:
	std::unordered_map<std::string, ShaderProgram> _storage;

public:
	ShaderStorage();

	ShaderProgram* getShaderProgram(const std::string& name);
	void addShaderProgram(const std::string& name, ShaderProgram shaderProgram);
	void removeShaderProgram(const std::string& name);
};