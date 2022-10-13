#include "ObjectManager.h"

ObjectManager::ObjectManager() {};

ObjectManager::~ObjectManager()
{
	for (auto o : objects)
	{
		delete o;
	}
	objects.clear();
};

void ObjectManager::addObject(Object* object)
{
	objects.push_back(object);
}

void ObjectManager::update()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
	}
}

void ObjectManager::draw(const glm::vec3& cameraPosition, ShaderProgram& opaqueShaderProgram, ShaderProgram& transparentShaderProgram)
{
	std::map<float, Object*> sorted;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isModelOpaque())
		{
			objects[i]->draw(opaqueShaderProgram);
		}
		else
		{
			float distance = glm::length(cameraPosition - objects[i]->getTranslation());
			sorted[distance] = objects[i];
		}
	}

	if (!sorted.empty())
	{
		//Sorting all half-transparent objects before rendering them
		for (std::map<float, Object*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, it->second->getTranslation());
			it->second->draw(transparentShaderProgram);
		}
	}
}