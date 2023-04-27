#include "LightManagerUI.h"

static int selectedPointLightIndex;
static int selectedSpotLightIndex;

LightManagerUI::LightManagerUI() {};

void LightManagerUI::setup(LightManager* lightManager)
{
	ptr_LightManager = lightManager;
	selectedPointLightIndex = 0;
	selectedSpotLightIndex = 0;
}

void LightManagerUI::draw()
{
	ImGui::Begin("LightManager");

	ImGui::Text("Directional Light");

	ImGui::SliderFloat3("Directional Light Direction", glm::value_ptr(ptr_LightManager->_directionalLight.direction), -100.0f, 100.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Position", glm::value_ptr(ptr_LightManager->_directionalLight.position), -100.0f, 100.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Ambient", glm::value_ptr(ptr_LightManager->_directionalLight.ambient), 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Diffuse", glm::value_ptr(ptr_LightManager->_directionalLight.diffuse), 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Specular", glm::value_ptr(ptr_LightManager->_directionalLight.specular), 0.0f, 1.0f, "%.3f", 0);

	if (ImGui::Button("Directional Light Switch"))
		ptr_LightManager->_isDirectionalLightActive = !ptr_LightManager->_isDirectionalLightActive;

	if(ptr_LightManager->_pointLights.size() != 0)
	{
		int pntLightIndex = selectedPointLightIndex;

		ImGui::SliderInt("Select Point Light", &selectedPointLightIndex, 0, ptr_LightManager->_pointLights.size() - 1);
		ImGui::SliderFloat3("Point Light Position", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex].position), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Ambient", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex].ambient), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Diffuse", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex].diffuse), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Specular", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex].specular), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Constant", &ptr_LightManager->_pointLights[pntLightIndex].constant, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Linear", &ptr_LightManager->_pointLights[pntLightIndex].linear, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Quadratic", &ptr_LightManager->_pointLights[pntLightIndex].quadratic, 0.0f, 1.0f, "%.3f", 0);
		if (ImGui::Button("Remove current Point Light"))
			ptr_LightManager->removePointLight(pntLightIndex);
	}
	if (ImGui::Button("Add Point Light"))
		ptr_LightManager->addPointLight(PointLight());

	if (ptr_LightManager->_spotLights.size() != 0)
	{
		int sptLightIndex = selectedSpotLightIndex;

		GLfloat tempSptCutOff = ptr_LightManager->_spotLights[sptLightIndex].getCutOffInDegrees();
		GLfloat tempSptOuterCutOff = ptr_LightManager->_spotLights[sptLightIndex].getOuterCutOffInDegrees();

		ImGui::SliderInt("Select Spot Light", &selectedSpotLightIndex, 0, ptr_LightManager->_spotLights.size() - 1);
		ImGui::SliderFloat3("Spot Light Position", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex].position), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Direction", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex].direction), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Ambient", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex].ambient), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Diffuse", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex].diffuse), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Specular", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex].specular), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Constant", &ptr_LightManager->_spotLights[sptLightIndex].constant, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Linear", &ptr_LightManager->_spotLights[sptLightIndex].linear, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Quadratic", &ptr_LightManager->_spotLights[sptLightIndex].quadratic, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Cut Off", &tempSptCutOff, 0.0f, 180.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Outer Cut Off", &tempSptOuterCutOff, 0.0f, 180.0f, "%.3f", 0);

		ptr_LightManager->_spotLights[sptLightIndex].setCutOffInDegrees(tempSptCutOff);
		ptr_LightManager->_spotLights[sptLightIndex].setOuterCutOffInDegrees(tempSptOuterCutOff);

		if (ImGui::Button("Remove current Spot Light"))
			ptr_LightManager->removeSpotLight(sptLightIndex);
	}
	if (ImGui::Button("Add Spot Light"))
		ptr_LightManager->addSpotLight(SpotLight());
	ImGui::End();
}