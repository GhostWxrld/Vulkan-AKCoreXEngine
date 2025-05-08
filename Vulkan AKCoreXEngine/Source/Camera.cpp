#include "../Headers/Camera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

glm::mat4 Camera::GetViewMatrix()
{
	//Calculate forward direction from angles
	glm::vec3 forward;
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	return glm::lookAt(position, position + forward, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::GetRotationMatrix(){
	glm::quat pitchRotation = glm::angleAxis(glm::radians(pitch), glm::vec3{ 1.f, 0.f, 0.f });
	glm::quat yawRotation = glm::angleAxis(glm::radians(yaw), glm::vec3{ 0.f, 1.f, 0.f });

	return glm::toMat4(yawRotation) * glm::toMat4(pitchRotation);
}

void Camera::ProcessCommands(GLFWwindow* window, float deltaTime){

	velocity = glm::vec3(0.f);		//Reset velocity each frame

	float speed = movementSpeed * deltaTime;

	float velocityAmount = movementSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		velocity.z += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		velocity.z -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		velocity.x -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		velocity.x += speed;
	}
}

void Camera::ProcessMouseMovements(float xOffset, float yOffset){
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	if (yaw > 360.0f) yaw -= 360.0f;
	if (yaw < 0.0f) yaw += 360.0f;
}

void Camera::Update(float deltaTime) {
	glm::vec3 forward = {
		cos (glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin (glm::radians(pitch)),
		sin (glm::radians(yaw)) * cos(glm::radians(pitch))
	};
	forward = glm::normalize(forward);

	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

	position += forward * velocity.z;
	position += right * velocity.x;
}
