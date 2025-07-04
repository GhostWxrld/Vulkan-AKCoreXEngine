#pragma once
#include <iostream>
#include <vulkan/vulkan.h>
#include <string> 
#include <vector>

typedef uint32_t u32;

class Cubemap{
public:
	Cubemap() = default;
	
	void CreateCubemapImage(u32 width, u32 height, u32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	VkImageView CreateCubemapImageView(VkDevice logicalDevice, VkFormat format, VkImageAspectFlags aspectFlags, u32 mipLevels);
	VkSampler CreateCubemapSampler(VkPhysicalDevice physicalDeviceRef, VkDevice logicalDeviceRef);
	void DestroyCubemap(VkDevice logicalDevice);
	void TransitionCubemapImageLayout(VkCommandBuffer commandBuffer, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, u32 mipLevel);
	void CopyBufferToCubemapImage(VkCommandBuffer commandBuffer, VkBuffer buffer, u32 width, u32 height, u32 layerIndex);
	u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
	void Initialize(VkPhysicalDevice physicalDeviceRef, VkDevice logicalDeviceRef);

	VkImage cubemapImage;
	VkDeviceMemory cubemapImageMemory;
	VkImageView cubemapImageView;
	VkSampler cubemapSampler;
private:
	VkMemoryRequirements cubemapMemRequirements;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;

};

