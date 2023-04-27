#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "model/Model.h"

class Object
{
public:
	Object();
	Object(const Model& _model, const glm::mat4& _modelMatrix = glm::mat4(1.0f), bool instancing = false);

	void rotate(GLfloat degrees, glm::vec3 axis);
	void scale(glm::vec3 axis);
	void translate(glm::vec3 axis);

	void draw(ShaderProgram& shaderProgram);
	
	glm::vec3 getScale() const;
	glm::quat getOrientation() const;
	glm::vec3 getTranslation() const;
	glm::mat4 getModelMatrix() const;

	void setModelMatrix(const glm::mat4& modelMatrix);

	bool isModelOpaque();

	bool _instancing;
private:
	glm::vec3 _scale;
	glm::quat _orientation;
	glm::vec3 _translation;
	glm::mat4 _modelMatrix;

	Model _model;
};