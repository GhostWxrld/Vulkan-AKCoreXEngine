#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera{
public:
	glm::vec3 velocity;
	glm::vec3 position;

	float pitch{ 0.f };
	float yaw{ 0.f };

	float movementSpeed = 5.0f;
	float mouseSensitivity = 0.1f;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetRotationMatrix();

	void ProcessCommands(GLFWwindow* window, float deltaTime);
	void ProcessMouseMovements(float xOffset, float yOffset);

	void Update(float deltaTime);

};

