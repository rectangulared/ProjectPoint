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

	ImGui::SliderFloat3("Directional Light Direction", glm::value_ptr(ptr_LightManager->_directionalLight._direction), -100.0f, 100.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Position", glm::value_ptr(ptr_LightManager->_directionalLight._position), -100.0f, 100.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Ambient", glm::value_ptr(ptr_LightManager->_directionalLight._ambient), 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Diffuse", glm::value_ptr(ptr_LightManager->_directionalLight._diffuse), 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Specular", glm::value_ptr(ptr_LightManager->_directionalLight._specular), 0.0f, 1.0f, "%.3f", 0);

	if (ImGui::Button("Directional Light Switch"))
		ptr_LightManager->_isDirectionalLightActive = !ptr_LightManager->_isDirectionalLightActive;

	if(ptr_LightManager->_pointLights.size() != 0)
	{
		int pntLightIndex = selectedPointLightIndex;

		ImGui::SliderInt("Select Point Light", &selectedPointLightIndex, 0, ptr_LightManager->_pointLights.size() - 1);
		ImGui::SliderFloat3("Point Light Position", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex]._position), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Ambient", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex]._ambient), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Diffuse", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex]._diffuse), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Specular", glm::value_ptr(ptr_LightManager->_pointLights[pntLightIndex]._specular), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Constant", &ptr_LightManager->_pointLights[pntLightIndex]._constant, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Linear", &ptr_LightManager->_pointLights[pntLightIndex]._linear, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Quadratic", &ptr_LightManager->_pointLights[pntLightIndex]._quadratic, 0.0f, 1.0f, "%.3f", 0);
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
		ImGui::SliderFloat3("Spot Light Position", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex]._position), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Direction", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex]._direction), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Ambient", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex]._ambient), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Diffuse", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex]._diffuse), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Specular", glm::value_ptr(ptr_LightManager->_spotLights[sptLightIndex]._specular), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Constant", &ptr_LightManager->_spotLights[sptLightIndex]._constant, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Linear", &ptr_LightManager->_spotLights[sptLightIndex]._linear, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Quadratic", &ptr_LightManager->_spotLights[sptLightIndex]._quadratic, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Cut Off", &tempSptCutOff, 0.0f, 180.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Outer Cut Off", &tempSptOuterCutOff, 0.0f, 180.0f, "%.3f", 0);

		ptr_LightManager->_spotLights[sptLightIndex].setCutOffInDegrees(tempSptCutOff);
		ptr_LightManager->_spotLights[sptLightIndex].setOuterCutOffInDegrees(tempSptOuterCutOff);

		if (ImGui::Button("Remove current Spot Light"))
			ptr_LightManager->removeSpotLight(sptLightIndex);
	}
	if (ImGui::Button("Add Spot Light"))
		ptr_LightManager->addSpotLight(SpotLight(true, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), new Texture(1024, 1024, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT)));
	ImGui::End();
}