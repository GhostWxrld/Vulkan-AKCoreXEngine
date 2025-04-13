#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define NOMINMAX

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>

typedef uint32_t u32;

const int MAX_FRAMES_IN_FLIGHT = 2;

class Renderer {
public:
	void Run();

	bool framebufferResized = false;
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
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateSyncObjects();

	//Main Loop Functions
	void DrawFrame();

	GLFWwindow* window;
	const u32 WIDTH = 1920;
	const u32 HEIGHT = 1080;

	VkInstance instance;


	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;

	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	//Command Buffers
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffer;

	//Synchronization
	std::vector<VkSemaphore> imageAvailableSemaphore;
	std::vector<VkSemaphore> renderFinishedSemaphore;
	std::vector<VkFence> inFlightFence;

	u32 currentFrame = 0;

	//Image View
	std::vector<VkImageView> swapChainImageViews;

	//Structs
	struct QueueFamilyIndices {
		std::optional<uint32_t>graphicsFamily;
		std::optional<uint32_t>presentationFamily;

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

