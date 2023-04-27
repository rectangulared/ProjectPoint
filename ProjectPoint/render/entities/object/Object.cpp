#include "Object.h"
#include "GLFW/glfw3.h"

Object::Object() :
	_instancing(0),
	_modelMatrix(glm::mat4{ 1.0f })
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(_modelMatrix, _scale, _orientation, _translation, skew, perspective);
}

Object::Object(const Model& _model, const glm::mat4& _modelMatrix, bool instancing) :
	_model(_model),
	_modelMatrix(_modelMatrix),
	_instancing(instancing)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(_modelMatrix, _scale, _orientation, _translation, skew, perspective);
};

void Object::rotate(GLfloat degrees, glm::vec3 axis)
{
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(degrees), axis);
}

void Object::scale(glm::vec3 axis)
{
	_modelMatrix = glm::scale(_modelMatrix, axis);
}

void Object::translate(glm::vec3 axis)
{
	_modelMatrix = glm::translate(_modelMatrix, axis);
}

void Object::draw(ShaderProgram& shaderProgram)
{
	shaderProgram.use();
	shaderProgram.setMat4f("model", _modelMatrix);
	_model.draw(shaderProgram);
}

glm::vec3 Object::getScale() const
{
	return _scale;
}

glm::quat Object::getOrientation() const
{
	return _orientation;
}

glm::vec3 Object::getTranslation() const
{
	return _translation;
}

glm::mat4 Object::getModelMatrix() const
{
	return _modelMatrix;
}

void Object::setModelMatrix(const glm::mat4& modelMatrix)
{
	_modelMatrix = modelMatrix;
}

bool Object::isModelOpaque()
{
	return _model.isOpaque();
}