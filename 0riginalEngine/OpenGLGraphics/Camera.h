#pragma once

#include "glm.hpp"
#include "ext.hpp"
#include "../glcore/gl_core_4_5.h"
#include "glfw3.h"

#define WIDTH 1080
#define HEIGHT 720

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{

public:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	bool firstMouse;

	Camera(glm::vec3 position = glm::vec3(-2, -2, 1), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float Yaw = YAW, float Pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float Yaw, float Pitch);

	glm::mat4 getViewMatrix();
	void processKeyboard(GLFWwindow* window, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void processMouseScroll(float yOffset);

private:
	void updateCameraVectors();
};

