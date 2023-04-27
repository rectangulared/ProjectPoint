#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "render/UI/LightManagerUI/LightManagerUI.h"
#include "render/entities/camera/Camera.h"
#include "render/entities/object/model/Model.h"
#include "render/managers/LightManager.h"
#include "render/managers/ObjectManager.h"
#include "render/entities/object/Object.h"
#include <render/entities/storage/shaderStorage/ShaderStorage.h>

class WorldManager
{
public:

	WorldManager(WorldManager& other) = delete;

	void operator=(const WorldManager&) = delete;

	static WorldManager* getInstance();

	void init();
	void update();
	void clear();

	void processInput();

protected:
	WorldManager();

	static WorldManager* _worldManager;

private:
	int SCREEN_WIDTH{ 1600 };
	int SCREEN_HEIGHT{ 900 };

	int SHADOW_MAP_WIDTH = 4096;
	int SHADOW_MAP_HEIGHT = 4096;

	bool firstMouse = true;
	float lastX = SCREEN_WIDTH / 2.0f;
	float lastY = SCREEN_HEIGHT / 2.0f;
	Camera camera;

	bool isMenuOpen = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	GLFWwindow* window;

	ShaderStorage shaderStorage;

	Model cubeModel;

	LightManager lightManager;
	LightManagerUI lightManagerUi;
	ObjectManager objectManager;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
		{
			_worldManager->isMenuOpen = !_worldManager->isMenuOpen;
			if (_worldManager->isMenuOpen)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		if (!_worldManager->isMenuOpen)
		{
			float xpos = static_cast<float>(xposIn);
			float ypos = static_cast<float>(yposIn);

			if (_worldManager->firstMouse)
			{
				_worldManager->lastX = xpos;
				_worldManager->lastY = ypos;
				_worldManager->firstMouse = false;
			}

			float xoffset = xpos - _worldManager->lastX;
			float yoffset = _worldManager->lastY - ypos;

			_worldManager->lastX = xpos;
			_worldManager->lastY = ypos;

			_worldManager->camera.processMouseMovement(xoffset, yoffset);
		}
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		_worldManager->camera.processMouseScroll(static_cast<float>(yoffset));
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		_worldManager->SCREEN_WIDTH = width;
		_worldManager->SCREEN_HEIGHT = height;
		glViewport(0, 0, width, height);
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {}
};