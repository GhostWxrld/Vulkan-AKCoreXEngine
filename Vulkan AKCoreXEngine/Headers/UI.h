#pragma once

#include <vector> 

#include "ImGui/imgui.h"
#include "ImGui/Backend/imgui_impl_glfw.h"
#include "ImGui/Backend/imgui_impl_vulkan.h"
#include <iostream>

#include <glm/glm.hpp>

typedef uint32_t u32;

class UI {
public:
	UI() = default;
	~UI() = default;

	struct LightObject {
		alignas(16) glm::vec4 direction;
		alignas(16) glm::vec4 color;
		alignas(4) float intensity;
		alignas(4) float ambient;
	};

	LightObject light;

	void BeginFrame(LightObject& light);
	void Cleanup(VkDevice logicalDevice);

	void UISettings(LightObject& light);
	void CreateUIDescriptorPool(VkDevice logicalDevice);
	void InitImGui(GLFWwindow* window, VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, u32 queueFamily, VkQueue queue, u32 minImageCount, u32 imageSize, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass);
	void CreateUICommandPool(VkDevice logicalDevice, u32 queueFamilyIndex);

private:

	VkCommandBuffer BeginSingleTimeCommands(VkDevice logicalDevice);
	void EndSingleTimeCommands(VkQueue queueSubmit);

	VkDevice logicalDevice;
	VkCommandBuffer commandBuffers;
	VkDescriptorPool m_descriptorPool;
	VkCommandPool commandPool;
	VkQueue queue;

};

