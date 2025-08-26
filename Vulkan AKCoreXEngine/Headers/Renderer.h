#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define NOMINMAX

#include "Camera.h"
#include "Cubemap.h"
#include "UI.h"

#include <tiny_obj_loader.h>

#include <print>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <limits>
#include <set> 
#include <algorithm>
#include <chrono> 
#include <unordered_map>
#include <sstream>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>

#include "../Source/stb_image.h"


typedef uint32_t u32;
typedef int32_t i32;
//typedef uint16_t u16;

const int MAX_FRAMES_IN_FLIGHT = 2;

//  ____                _                           
// |  _ \ ___ _ __   __| | ___ _ __                 
// | |_) / _ \ '_ \ / _` |/ _ \ '__|                
// |  _ <  __/ | | | (_| |  __/ |                   
// |_|_\_\___|_| |_|\__,_|\___|_|                   
// |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ ___ 
// | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __/ __|
// |  _|| |_| | | | | (__| |_| | (_) | | | \__ \__ \
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/___/
class Renderer {
public:

	Camera mainCamera;
	Cubemap skybox;
	UI ui;
	UI::LightObject light;

	void Run();

	bool framebufferResized = false;
	int frameCount = 0;
	double previousTime = glfwGetTime();

private:
	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void Cleanup();
	void CreateInstance();
	void SetupDebugMessenger();
	void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateImageViews();
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, u32 mipLevels);
	void CreateRenderPass();
	void CreateDescriptorLayout();
	void CreateGraphicsPipeline();
	void CreateSkyboxPipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateColorResources();
	void CreateDepthResources();
	void CreateTextureImage();
	void CreateCubemapImages();
	void CreateTextureImageView();
	void CreateTextureSampler();
	void LoadModel();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateCommandBuffers();
	void CreateSyncObjects();


	//Main Loop Functions
	void DrawFrame();

	GLFWwindow* window;
	const u32 WIDTH = 2560;
	const u32 HEIGHT = 1440;

	std::string MODEL_PATH = "E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Models/viking_room.obj";
	std::string GROUND_MODEL = "E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Models/ground.obj";

	const std::string TEXTURE_PATH = "E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Textures/viking_room.png";
	const std::vector<std::string> cubemapPaths = {
		"E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Cubemap/Space/px.png",
		"E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Cubemap/Space/nx.png",
		"E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Cubemap/Space/py.png",
		"E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Cubemap/Space/ny.png",
		"E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Cubemap/Space/pz.png",
		"E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Cubemap/Space/nz.png"
	};

	VkInstance instance;


	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;



	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;

	//CUBEMAP Pipeline
	VkPipeline skyboxPipeline;
	VkPipelineLayout skyboxPipelineLayout;


	std::vector<VkFramebuffer> swapChainFramebuffers;

	//Descriptor Sets
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayout skyboxDescriptorSetLayout;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	//Command Buffers
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffer;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	//Skybox Buffers
	VkBuffer skyboxVertexBuffer;
	VkDeviceMemory skyboxVertexBufferMemory;


	//Light Buffer and Light Buffer Memory
	std::vector<VkBuffer> lightBuffer;
	std::vector<VkDeviceMemory> lightBufferMemory;
	std::vector<void*> lightBufferMapped;
	

	VkMemoryRequirements memRequirements;

	//UniformBuffers
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBufferMemory;
	std::vector<void*> uniformBuffersMapped;

	//Synchronization
	std::vector<VkSemaphore> imageAvailableSemaphore;
	std::vector<VkSemaphore> renderFinishedSemaphore;
	std::vector<VkFence> inFlightFence;
	std::vector<VkFence> imagesInFlight;

	u32 currentFrame = 0;

	//Image View
	std::vector<VkImageView> swapChainImageViews;

	//Texture Setup
	u32 mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkImageView textureImageView;
	VkSampler textureSampler;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	private:

	std::vector<Vertex> vertices;
	std::vector<u32>indices;

	struct UniformBufferObject {
		 glm::mat4 model;
		 glm::mat4 view;
		 glm::mat4 proj;
	};

	//END OF SHADER STUFF-----------------------

	struct QueueFamilyIndices {
		std::optional<u32> graphicsFamily;
		std::optional<u32> presentationFamily;
		bool IsComplete() {
			return graphicsFamily.has_value() && presentationFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	//Swap Chain setup
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	
	//Texture Functions
	void CreateImage(u32 width, u32 height, u32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels);

	//Helper Functions
	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	static std::vector<char> ReadFile(const std::string& fileName);
	VkShaderModule CreateShaderModule(const std::vector<char>& code, const std::string& name);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);
	void CleanupSwapchain();
	void RecreateSwapchain();
	u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void UpdateUniformBuffer(u32 currentImage);
	void CopyBufferToImage(VkBuffer buffer, VkImage  image, u32 width, u32 height);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);
	void UpdateFPSCounter(GLFWwindow* window);			//FPS counter on top left

	//MipMap Func
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, i32 texWidth, i32 texHeight, u32 mipLevels);

	//MSAA
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkSampleCountFlagBits GetMaxUsableSampleCount();

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	//Cubemap
	void CreateSkyboxResources();
	

	//UI
	void InitImgui();


	// __     __    _ _     _       _   _             
	// \ \   / /_ _| (_) __| | __ _| |_(_) ___  _ __  
	//  \ \ / / _` | | |/ _` |/ _` | __| |/ _ \| '_ \ 
	//   \ V / (_| | | | (_| | (_| | |_| | (_) | | | |
	//  _ \_/ \__,_|_|_|\__,_|\__,_|\__|_|\___/|_| |_|
	// | |    __ _ _   _  ___ _ __ ___                
	// | |   / _` | | | |/ _ \ '__/ __|               
	// | |__| (_| | |_| |  __/ |  \__ \               
	// |_____\__,_|\__, |\___|_|  |___/               
	//             |___/                          
	// 

	VkDebugUtilsMessengerEXT debugMessenger;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif //Enables Validation Layers for Debugging only

	bool CheckValidationSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> GetRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layers: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

};

