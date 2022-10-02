#include "LightManagerUI.h"

static int selectedPointLightIndex;
static int selectedSpotLightIndex;


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
	glm::vec3 tempDirDir = ptr_LightManager->getDirectionalLight()->getDirection();
	glm::vec3 tempDirAmbient = ptr_LightManager->getDirectionalLight()->getAmbient();
	glm::vec3 tempDirDiffuse = ptr_LightManager->getDirectionalLight()->getDiffuse();
	glm::vec3 tempDirSpecular = ptr_LightManager->getDirectionalLight()->getSpecular();

	ImGui::SliderFloat3("Directional Light Direction", glm::value_ptr(tempDirDir), -100.0f, 100.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Ambient", glm::value_ptr(tempDirAmbient), 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Diffuse", glm::value_ptr(tempDirDiffuse), 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("Directional Light Specular", glm::value_ptr(tempDirSpecular), 0.0f, 1.0f, "%.3f", 0);

	if (ImGui::Button("Directional Light Switch")) 
		ptr_LightManager->directionalLightSwitch();

	ptr_LightManager->changeDirectionalLight(DirectionalLight(tempDirDir, tempDirAmbient, tempDirDiffuse, tempDirSpecular));

	if(ptr_LightManager->getActivePointLights() != 0)
	{
		int pntLightIndex = selectedPointLightIndex;
		glm::vec3 tempPntPos = ptr_LightManager->getPointLightByIndex(pntLightIndex)->getPosition();

		glm::vec3 tempPntAmbient = ptr_LightManager->getPointLightByIndex(pntLightIndex)->getAmbient();
		glm::vec3 tempPntDiffuse = ptr_LightManager->getPointLightByIndex(pntLightIndex)->getDiffuse();
		glm::vec3 tempPntSpecular = ptr_LightManager->getPointLightByIndex(pntLightIndex)->getSpecular();

		GLfloat tempPntConstant = ptr_LightManager->getPointLightByIndex(pntLightIndex)->getConstant();
		GLfloat tempPntLinear = ptr_LightManager->getPointLightByIndex(pntLightIndex)->getLinear();
		GLfloat tempPntQuadratic = ptr_LightManager->getPointLightByIndex(pntLightIndex)->getQuadratic();

		ImGui::SliderInt("Select Point Light", &selectedPointLightIndex, 0, ptr_LightManager->getActivePointLights() - 1);
		ImGui::SliderFloat3("Point Light Position", glm::value_ptr(tempPntPos), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Ambient", glm::value_ptr(tempPntAmbient), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Diffuse", glm::value_ptr(tempPntDiffuse), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Point Light Specular", glm::value_ptr(tempPntSpecular), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Constant", &tempPntConstant, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Linear", &tempPntLinear, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Point Light Quadratic", &tempPntQuadratic, 0.0f, 1.0f, "%.3f", 0);
		ptr_LightManager->changePointLight(pntLightIndex, PointLight(tempPntPos, tempPntAmbient, tempPntDiffuse, tempPntSpecular, tempPntConstant, tempPntLinear, tempPntQuadratic));
		if (ImGui::Button("Remove current Point Light"))
			ptr_LightManager->removePointLight(pntLightIndex);
	}
	if (ImGui::Button("Add Point Light"))
		ptr_LightManager->addPointLight();

	if (ptr_LightManager->getActiveSpotLights() != 0)
	{
		int sptLightIndex = selectedSpotLightIndex;
		glm::vec3 tempSptPos = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getPosition();
		glm::vec3 tempSptDir = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getDirection();
		glm::vec3 tempSptAmbient = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getAmbient();
		glm::vec3 tempSptDiffuse = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getDiffuse();
		glm::vec3 tempSptSpecular = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getSpecular();

		GLfloat tempSptConstant = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getConstant();
		GLfloat tempSptLinear = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getLinear();
		GLfloat tempSptQuadratic = ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getQuadratic();
		GLfloat tempSptCutOff = glm::degrees(glm::acos(ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getCutOff()));
		GLfloat tempSptOuterCutOff = glm::degrees(glm::acos(ptr_LightManager->getSpotLightByIndex(sptLightIndex)->getOuterCutOff()));

		ImGui::SliderInt("Select Spot Light", &selectedSpotLightIndex, 0, ptr_LightManager->getActiveSpotLights() - 1);
		ImGui::SliderFloat3("Spot Light Position", glm::value_ptr(tempSptPos), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Direction", glm::value_ptr(tempSptDir), -100.0f, 100.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Ambient", glm::value_ptr(tempSptAmbient), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Diffuse", glm::value_ptr(tempSptDiffuse), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat3("Spot Light Specular", glm::value_ptr(tempSptSpecular), 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Constant", &tempSptConstant, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Linear", &tempSptLinear, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Quadratic", &tempSptQuadratic, 0.0f, 1.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Cut Off", &tempSptCutOff, 0.0f, 180.0f, "%.3f", 0);
		ImGui::SliderFloat("Spot Light Outer Cut Off", &tempSptOuterCutOff, 0.0f, 180.0f, "%.3f", 0);
		ptr_LightManager->changeSpotLight(sptLightIndex, SpotLight(tempSptPos, tempSptDir, tempSptAmbient, tempSptDiffuse, tempSptSpecular, tempSptConstant, tempSptLinear, tempSptQuadratic, tempSptCutOff, tempSptOuterCutOff));
		if (ImGui::Button("Remove current Spot Light"))
			ptr_LightManager->removeSpotLight(sptLightIndex);
	}
	if (ImGui::Button("Add Spot Light"))
		ptr_LightManager->addSpotLight();
	ImGui::End();
}