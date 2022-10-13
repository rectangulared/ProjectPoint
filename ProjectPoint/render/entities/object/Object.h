#pragma once

#include "bullet/btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "model/Model.h"

class Object
{
public:
	enum CollisionShape
	{
		CUBE = 0,
		PLANE = 1
	};

	Object();
	Object(const Model& _model, CollisionShape shape, const glm::mat4 _modelMatrix = glm::mat4(1.0f), btRigidBody* _rigidBody = nullptr);
	Object(const Model& _model, CollisionShape shape, const glm::mat4 _modelMatrix, btRigidBody* _rigidBody, btVector3 boxHalfExtends, btVector3 origin, float mass);

	void rotate(GLfloat degrees, glm::vec3 axis);
	void Scale(glm::vec3 axis);
	void translate(glm::vec3 axis);

	void update();
	void draw(ShaderProgram& shaderProgram);
	
	bool getIsEternal() const;
	void setIsEternal(bool isEternal);
	double getSpawnTime() const;
	glm::vec3 getScale() const;
	glm::quat getOrientation() const;
	glm::vec3 getTranslation() const;
	glm::vec3 getSkew() const;
	glm::vec4 getPerspective() const;
	glm::mat4 getModelMatrix() const;
	btRigidBody* getRigidBodyPrt();

	void addBoxShape(btVector3 boxHalfExtents, btVector3 origin, float mass);
	void addInfinitePlaneShape(btVector3 Normals, btVector3 origin);

	void setModelMatrix(const glm::mat4& model);


	bool isModelOpaque();

private:
	bool isEternal;
	double spawnTime;
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::mat4 modelMatrix;
	btRigidBody* rigidBody;
	CollisionShape collisionShape;
	Model model;
};