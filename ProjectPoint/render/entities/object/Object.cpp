#include "Object.h"
#include "GLFW/glfw3.h"

Object::Object() : modelMatrix(glm::mat4{ 1.0f }), spawnTime(glfwGetTime()), isEternal(false)
{
	glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
}

Object::Object(const Model& _model, CollisionShape shape, const glm::mat4 _modelMatrix, btRigidBody* _rigidBody)
	: model(_model), modelMatrix(_modelMatrix), collisionShape(shape), rigidBody(_rigidBody), spawnTime(glfwGetTime()), isEternal(false)
{
	glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
	if (collisionShape == CUBE)
	{
		this->addBoxShape(btVector3(1.0f, 1.0f, 1.0f), btVector3(0.0f, 20.0f, 0.0f), 10.0f);
	}
	if (collisionShape == PLANE)
	{
		this->addInfinitePlaneShape(btVector3(0.0f, 1.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	}
};

Object::Object(const Model& _model, CollisionShape shape, const glm::mat4 _modelMatrix, btRigidBody* _rigidBody, btVector3 boxHalfExtends, btVector3 origin, float mass)
	: model(_model), modelMatrix(_modelMatrix), collisionShape(shape), rigidBody(_rigidBody), spawnTime(glfwGetTime()), isEternal(false)
{
	glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
	this->addBoxShape(boxHalfExtends, origin, mass);
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

void Object::update()
{
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	glm::mat4 mat;
	t.getOpenGLMatrix(glm::value_ptr(mat));
	modelMatrix = glm::scale(mat, scale);
}

void Object::draw(ShaderProgram& shaderProgram)
{
	glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective);
	shaderProgram.use();
	shaderProgram.setMat4f("model", modelMatrix);
	model.draw(shaderProgram);
}

bool Object::getIsEternal() const
{
	return isEternal;
}

void Object::setIsEternal(bool isEternal)
{
	this->isEternal = isEternal;
}

double Object::getSpawnTime() const
{
	return spawnTime;
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

btRigidBody* Object::getRigidBodyPrt()
{
	return rigidBody;
}

void Object::addBoxShape(btVector3 boxHalfExtents, btVector3 origin, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(origin);
	btBoxShape* box = new btBoxShape(boxHalfExtents);
	btVector3 inertia(0, 0, 0);
	if (mass != 0)
	{
		box->calculateLocalInertia(mass, inertia);
	}
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
    rigidBody = new btRigidBody(info);
}

void Object::addInfinitePlaneShape(btVector3 Normals, btVector3 origin)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(btScalar(0), motion, plane);
	rigidBody = new btRigidBody(info);
}

void Object::setModelMatrix(const glm::mat4& model)
{
	modelMatrix = model;
}

bool Object::isModelOpaque()
{
	return model.isOpaque();
}