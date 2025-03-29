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
}

void Renderer::MainLoop(){
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void Renderer::Cleanup(){
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
	appInfo.apiVersion = VK_API_VERSION_1_4;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Debug Info
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	
	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	//Set Validation Layers names if they are enabled
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
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
