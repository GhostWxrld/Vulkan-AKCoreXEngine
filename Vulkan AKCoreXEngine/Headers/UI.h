#pragma once

#include <vector> 
#include <vulkan/vulkan.h>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <iostream>


typedef uint32_t u32;

class UI {
public:
	UI() = default;
	~UI() = default;

	void BeginFrame();
	void Cleanup(VkDevice logicalDevice);

	void UISettings();
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

