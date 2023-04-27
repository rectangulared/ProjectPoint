#include "ObjectManager.h"

ObjectManager::ObjectManager() {};

ObjectManager::~ObjectManager()
{
	for (auto o : _objects)
	{
		delete o;
	}
	_objects.clear();
};

void ObjectManager::addObject(Object* object)
{
	_objects.push_back(object);
}

void ObjectManager::draw(const glm::vec3& cameraPosition, ShaderProgram& opaqueShaderProgram, ShaderProgram& transparentShaderProgram, ShaderProgram& instancingShaderProgram)
{
	std::map<float, Object*> sorted;
	for (size_t i = 0; i < _objects.size(); i++)
	{
		if (_objects[i]->isModelOpaque() && !_objects[i]->_instancing)
		{
			_objects[i]->draw(opaqueShaderProgram);
		}
		else if (_objects[i]->_instancing)
		{
			_objects[i]->draw(instancingShaderProgram);
		}
		else
		{
			float distance = glm::length(cameraPosition - _objects[i]->getTranslation());
			sorted[distance] = _objects[i];
		}
	}

	if (!sorted.empty())
	{
		//Sorting all half-transparent objects before rendering them
		for (std::map<float, Object*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			_model = glm::translate(_model, it->second->getTranslation());
			it->second->draw(transparentShaderProgram);
		}
	}
}