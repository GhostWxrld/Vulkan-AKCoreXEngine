#pragma once

#include "Macros.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Camera{
public:
	Camera() = default;
	glm::vec3 velocity;
	glm::vec3 position = glm::vec3(0.0f);

	float pitch{ 0.f };
	float yaw{ 0.f };

	float movementSpeed = 5.0f;
	float mouseSensitivity = 0.1f;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetRotationMatrix();

	glm::vec3 GetPosition();

	void ProcessCommands(GLFWwindow* window, float deltaTime);
	void ProcessMouseMovements(float xOffset, float yOffset);

	void Update(float deltaTime);

	bool mouseLock = true;
};

