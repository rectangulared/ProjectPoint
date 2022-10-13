#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr static GLfloat YAW = -90.0f;
constexpr static GLfloat PITCH = 0.0f;
constexpr static GLfloat SPEED = 2.5f;
constexpr static GLfloat SENSITIVITY = 0.1f;
constexpr static GLfloat FOV = 78.0f;

enum CameraMovement 
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat fov;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat _yaw = YAW, GLfloat _pitch = PITCH);

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat _yaw, GLfloat _pitch);

    glm::mat4 getViewMatrix();

    void processKeyboard(CameraMovement direction, GLfloat deltaTime);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat yoffset);

private:
    void updateCameraVectors();
};