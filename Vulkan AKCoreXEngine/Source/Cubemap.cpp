#include "../Headers/Cubemap.h"

//TODO: MAKE SURE TO FIX THE CUBEMAP, ORIENTATION IS OFF AND FACES ARE NOT THERE

void Cubemap::CreateCubemapImage(u32 width, u32 height, u32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) {

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 6;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSamples;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

	VkResult result = vkCreateImage(logicalDevice, &imageInfo, nullptr, &cubemapImage);
	if (result != VK_SUCCESS) {
		std::cerr << "Failed to create Cubemap Image!" << std::endl;
		throw std::runtime_error("Failed to create Cubemap Image!");
	}

	vkGetImageMemoryRequirements(logicalDevice, cubemapImage, &cubemapMemRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = cubemapMemRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(cubemapMemRequirements.memoryTypeBits, properties);

	result = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &cubemapImageMemory);
	if (result != VK_SUCCESS) {
		std::cerr << "Failed to allocate Cubemap Image Memory!" << std::endl;
		throw std::runtime_error("Failed to allocate Cubemap Image Memory!");
	}
	else {
		std::cout << "Cubemap Image Memory Allocated -> " << cubemapImageMemory << std::endl;
	}

	vkBindImageMemory(logicalDevice, cubemapImage, cubemapImageMemory, 0);
}

VkImageView Cubemap::CreateCubemapImageView(VkDevice logicalDeviceRef, VkFormat format, VkImageAspectFlags aspectFlags, u32 mipLevels) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = cubemapImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 6;

	VkResult result = vkCreateImageView(logicalDeviceRef, &viewInfo, nullptr, &cubemapImageView);
	if (result != VK_SUCCESS) {
		std::cerr << "Failed to create Cubemap Image View!" << std::endl;
		throw std::runtime_error("Failed to create Cubemap Image View!");
	}
	else {
		std::cout << "Cubemap Image View Created -> " << cubemapImageView << std::endl;
	}

	return cubemapImageView;
}

VkSampler Cubemap::CreateCubemapSampler(VkPhysicalDevice physicalDeviceRef, VkDevice logicalDeviceRef) {
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physicalDeviceRef, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = VK_LOD_CLAMP_NONE; // All mipmap levels available

	VkResult result = vkCreateSampler(logicalDeviceRef, &samplerInfo, nullptr, &cubemapSampler);
	if (result != VK_SUCCESS) {
		std::cerr << "Failed to create Cubemap Sampler!" << std::endl;
		throw std::runtime_error("Failed to create texture sampler");
	}
	else {
		std::cout << "Cubemap Sampler Created -> " << cubemapSampler << std::endl;
	}

	return cubemapSampler;
}

void Cubemap::TransitionCubemapImageLayout(VkCommandBuffer commandBuffer, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, u32 mipLevel) {
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldImageLayout;
	barrier.newLayout = newImageLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = cubemapImage;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mipLevel;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 6;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags dstStage;

	if (oldImageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
		newImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		std::cerr << "Unsupported Cubemap Transition Layout!" << std::endl;
		throw std::runtime_error("Unsupported Cubemap Transition Layout!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage,
		dstStage,
		0,
		0,
		nullptr,
		0,
		nullptr,
		1, &barrier);
}

void Cubemap::CopyBufferToCubemapImage(VkCommandBuffer commandBuffer, VkBuffer buffer, u32 width, u32 height, u32 layerIndex) {
	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = layerIndex;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { static_cast<u32>(width), static_cast<u32>(height), 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, cubemapImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

u32 Cubemap::FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties)
{

	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (u32 i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	throw std::runtime_error("Failed to find suitable memory type!");
}

void Cubemap::Initialize(VkPhysicalDevice physicalDeviceRef, VkDevice logicalDeviceRef){
	physicalDevice = physicalDeviceRef; 
	logicalDevice = logicalDeviceRef;
}

void Cubemap::DestroyCubemap(VkDevice logicalDeviceRef) {
	if (cubemapSampler != VK_NULL_HANDLE) {
		vkDestroySampler(logicalDeviceRef, cubemapSampler, nullptr);
		cubemapSampler = VK_NULL_HANDLE;
	}
	if (cubemapImageView != VK_NULL_HANDLE) {
		vkDestroyImageView(logicalDeviceRef, cubemapImageView, nullptr);
		cubemapImageView = VK_NULL_HANDLE;
	}
	if (cubemapImage != VK_NULL_HANDLE) {
		vkDestroyImage(logicalDeviceRef, cubemapImage, nullptr);
		cubemapImage = VK_NULL_HANDLE;
	}
	if (cubemapImageMemory != VK_NULL_HANDLE) {
		vkFreeMemory(logicalDeviceRef, cubemapImageMemory, nullptr);
		cubemapImageMemory = VK_NULL_HANDLE;
	}
}
