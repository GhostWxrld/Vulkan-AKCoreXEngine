#include "../Headers/Renderer.h"

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
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

}

//Validation Function to Destroy the debug messages
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

//  ____                _                           
// |  _ \ ___ _ __   __| | ___ _ __                 
// | |_) / _ \ '_ \ / _` |/ _ \ '__|                
// |  _ <  __/ | | | (_| |  __/ |                   
// |_|_\_\___|_| |_|\__,_|\___|_|                   
// |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ ___ 
// | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __/ __|
// |  _|| |_| | | | | (__| |_| | (_) | | | \__ \__ \
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/___/
void Renderer::Run(){
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void Renderer::InitWindow(){
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "AKCoreXEngine", nullptr, nullptr);
}

void Renderer::InitVulkan(){
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	PickPhysicalDevice(); 
	CreateLogicalDevice();
	CreateSwapChain();
	CreateImageViews();
	CreateGraphicsPipeline();
}

void Renderer::MainLoop(){
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void Renderer::Cleanup(){

	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	}
	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(logicalDevice, nullptr);

	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::CreateInstance() {

	//VALIDATIION CHECK
	if (enableValidationLayers && !CheckValidationSupport()) {
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
	appInfo.pApplicationName = "AKCoreXEngine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "AKCoreXEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Debug Info
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	
	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	//Set Validation Layers names if they are enabled
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

	}else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	//Check that Instance is Working
	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance!");
	} else {
		std::cout << "=========================================" << std::endl;
		std::cout << "Vulkan instance created successfully!" << std::endl;
	}
}

void Renderer::SetupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to set up debug messenger!");
	}
}

void Renderer::CreateSurface(){
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to Create window Surface");
	}else {
		std::cout << "Window Surface Created-> " << surface << std::endl;
	}
}

void Renderer::PickPhysicalDevice(){
	u32 deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	//Check for devices available
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice>  devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;

			//Just to retrieve the name of the GPU
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

			std::cout << "Physical Device Retrieved->" << deviceProperties.deviceName << std::endl;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

void Renderer::CreateLogicalDevice(){
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<u32> uniqueQueueFamilies = { indices.graphicsFamily.value(),
											   indices.presentationFamily.value()};

	float queuePriority = 1.0f;
	for (u32 queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	//Logical Device Features
	VkPhysicalDeviceFeatures deviceFeatures{};

	//Logical Device Creation Info
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}else {
		createInfo.enabledLayerCount = 0;
	}
	
	VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to Create logical Device");
	}else {
		std::cout << "Logical Device Created-> "<< logicalDevice << std::endl;
	}

	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentationFamily.value(), 0, &presentQueue);
}

void Renderer::CreateSwapChain(){
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;

	//Check to not exceed the max image count
	//0 is a special value that means 
	//There is no maximum
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	u32 queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentationFamily.value() };
	if (indices.graphicsFamily != indices.presentationFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Swap Chain");
	}else {
		std::cout << "Swap Chain Created-> " << swapChain << std::endl;
	} 

	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

	//Store the Format and Extent for later use
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void Renderer::CreateImageViews(){
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Image Views");
		}
	}
	std::cout << "Image Views Created-> " << swapChainImages.size() << std::endl;
}

void Renderer::CreateGraphicsPipeline(){
	auto vertexShaderCode = ReadFile("E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Shaders/VertexShader.spv");
	auto fragmentShaderCode = ReadFile("E:/Vulkan Projects/Vulkan AKCoreXEngine/Vulkan AKCoreXEngine/Shaders/FragmentShader.spv");

	VkShaderModule vertShaderModule = CreateShaderModule(vertexShaderCode, "Vertex");
	VkShaderModule fragShaderModule = CreateShaderModule(fragmentShaderCode, "Fragment");

	vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	vertShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
	fragShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
}

VkSurfaceFormatKHR Renderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
			&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	//For now we settle for the first one
	//Later we will implement a check
	//To determine the best score and 
	//Choose based on that.
	return availableFormats[0];
}

VkPresentModeKHR Renderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities){
	if (capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
		return capabilities.currentExtent;
	}else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<u32>(width),
			static_cast<u32>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
										capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
										capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

//Checks if the device is suitable for the application
//For now we will settle for any GPU but in the future will change 
//To implement a more robust check based 
//on the requirements of the application
//And the highest score of the GPU
bool Renderer::IsDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices indices = FindQueueFamilies(device);

	//For Swap chain
	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

bool Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice device){
	u32 extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
} 

Renderer::QueueFamilyIndices Renderer::FindQueueFamilies(VkPhysicalDevice device){
	QueueFamilyIndices indices;

	u32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {

		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		if (presentationSupport) {
			indices.presentationFamily = i;
		}

		if (indices.IsComplete()) {
			break;
		}
		i++;
	}

	return indices; 
}

Renderer::SwapChainSupportDetails Renderer::QuerySwapChainSupport(VkPhysicalDevice device){
	
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	//Format Modes
	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	//Present Modes
	u32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

std::vector<char> Renderer::ReadFile(const std::string& fileName){
	
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

VkShaderModule Renderer::CreateShaderModule(const std::vector<char>& code, const std::string& shaderName){
	
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const u32*>(code.data());

	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Shader Module");
	}else {
		std::cout << shaderName << " Shader Module created!" << std::endl;
	}

	return shaderModule;
}

void Renderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo){
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallBack;
} 
