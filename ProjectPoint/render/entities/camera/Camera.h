#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr static float YAW = -90.0f;
constexpr static float PITCH = 0.0f;
constexpr static float SPEED = 2.5f;
constexpr static float SENSITIVITY = 0.1f;
constexpr static float FOV = 78.0f;

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

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float fov;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw, float _pitch);

    glm::mat4 getViewMatrix();

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};