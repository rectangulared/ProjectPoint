#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../../managers/LightManager.h"

class LightManagerUI
{
public:
	void setup(LightManager* lightManager);
	void draw();
	void switchVisibility();
private:
	LightManager* ptr_LightManager;
	bool isVisible;
	int selectedPointLightIndex;
};
