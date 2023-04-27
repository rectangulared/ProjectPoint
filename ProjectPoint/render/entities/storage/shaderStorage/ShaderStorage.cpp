#include "ShaderStorage.h"

ShaderStorage::ShaderStorage() {};

ShaderProgram* ShaderStorage::getShaderProgram(const std::string& name)
{
	if (_storage.find(name) == _storage.end())
	{
		//TODO::Error check
		return nullptr;
	}
	else
	{
		return &_storage[name];
	}
}

void ShaderStorage::addShaderProgram(const std::string& name, ShaderProgram shaderProgram)
{
	_storage.insert({ name, shaderProgram });
}

void ShaderStorage::removeShaderProgram(const std::string& name)
{
	if (_storage.find(name) == _storage.end())
	{
		//TODO::Error check
	}
	else
	{
		_storage.erase(name);
	}
}