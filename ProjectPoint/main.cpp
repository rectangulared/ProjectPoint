#include "render/managers/WorldManager.h"

int main()
{
	WorldManager* worldManager = WorldManager::getInstance();
	worldManager->init();
	worldManager->update();
	worldManager->clear();
}