#include "Object.h"
#include "GLFW/glfw3.h"

Object::Object() : modelMatrix(glm::mat4{ 1.0f })
{
	glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
}

Object::Object(const Model& _model, const glm::mat4 _modelMatrix) : model(_model), modelMatrix(_modelMatrix)
{
	glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
};

void Object::rotate(GLfloat degrees, glm::vec3 axis)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), axis);
}

void Object::Scale(glm::vec3 axis)
{
	modelMatrix = glm::scale(modelMatrix, axis);
}

void Object::translate(glm::vec3 axis)
{
	modelMatrix = glm::translate(modelMatrix, axis);
}

void Object::draw(ShaderProgram& shaderProgram)
{
	shaderProgram.use();
	shaderProgram.setMat4f("model", modelMatrix);
	model.draw(shaderProgram);
}

glm::vec3 Object::getScale() const
{
	return scale;
}

glm::quat Object::getOrientation() const
{
	return orientation;
}

glm::vec3 Object::getTranslation() const
{
	return translation;
}

glm::vec3 Object::getSkew() const
{
	return skew;
}

glm::vec4 Object::getPerspective() const
{
	return perspective;
}

glm::mat4 Object::getModelMatrix() const
{
	return modelMatrix;
}

void Object::setModelMatrix(const glm::mat4& model)
{
	modelMatrix = model;
}

bool Object::isModelOpaque()
{
	return model.isOpaque();
}