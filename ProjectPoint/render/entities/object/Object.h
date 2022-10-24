#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "model/Model.h"

class Object
{
public:
	Object();
	Object(const Model& _model, const glm::mat4 _modelMatrix = glm::mat4(1.0f));

	void rotate(GLfloat degrees, glm::vec3 axis);
	void Scale(glm::vec3 axis);
	void translate(glm::vec3 axis);

	void draw(ShaderProgram& shaderProgram);
	
	glm::vec3 getScale() const;
	glm::quat getOrientation() const;
	glm::vec3 getTranslation() const;
	glm::vec3 getSkew() const;
	glm::vec4 getPerspective() const;
	glm::mat4 getModelMatrix() const;

	void setModelMatrix(const glm::mat4& model);

	bool isModelOpaque();

private:
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::mat4 modelMatrix;
	Model model;
};