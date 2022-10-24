#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../../render/UI/LightManagerUI/LightManagerUI.h"
#include "../../render/entities/camera/Camera.h"
#include "../../render/entities/object/model/Model.h"
#include "../../render/managers/LightManager.h"
#include "../../render/managers/ObjectManager.h"
#include "../../render/entities/object/Object.h"

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

	static WorldManager* worldManager_;

private:
	int SCREEN_WIDTH{ 1600 };
	int SCREEN_HEIGHT{ 900 };

	bool firstMouse = true;
	float lastX = SCREEN_WIDTH / 2.0f;
	float lastY = SCREEN_HEIGHT / 2.0f;
	Camera camera;

	bool isMenuOpen = false;
	bool isItemSpawned = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	GLFWwindow* window;

	ShaderProgram mainShader;
	ShaderProgram opacityShader;

	Model cubeModel;

	LightManager lightManager;
	LightManagerUI lightManagerUi;
	ObjectManager objectManager;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		{
			worldManager_->isItemSpawned = !worldManager_->isItemSpawned;
		}

		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
		{
			worldManager_->isMenuOpen = !worldManager_->isMenuOpen;
			if (worldManager_->isMenuOpen)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		if (!worldManager_->isMenuOpen)
		{
			float xpos = static_cast<float>(xposIn);
			float ypos = static_cast<float>(yposIn);

			if (worldManager_->firstMouse)
			{
				worldManager_->lastX = xpos;
				worldManager_->lastY = ypos;
				worldManager_->firstMouse = false;
			}

			float xoffset = xpos - worldManager_->lastX;
			float yoffset = worldManager_->lastY - ypos;

			worldManager_->lastX = xpos;
			worldManager_->lastY = ypos;

			worldManager_->camera.processMouseMovement(xoffset, yoffset);
		}
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		worldManager_->camera.processMouseScroll(static_cast<float>(yoffset));
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		worldManager_->SCREEN_WIDTH = width;
		worldManager_->SCREEN_HEIGHT = height;
		glViewport(0, 0, width, height);
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {}
};