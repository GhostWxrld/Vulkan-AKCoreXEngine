#include "../Headers/UI.h"


//We replicate the BegingSingleTimeCommands and EndSingleTimeCommandns from Renderer.h
//This is to keep all UI stuff in the UI class

void UI::BeginFrame(LightObject& light){
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	UISettings(light);
}


void UI::CreateUIDescriptorPool(VkDevice logicalDevice) {

	std::vector<VkDescriptorPoolSize> poolSizes = {
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo poolCreateInfo{};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolCreateInfo.maxSets = 1000 * static_cast<u32>(poolSizes.size());
	poolCreateInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
	poolCreateInfo.pPoolSizes = poolSizes.data();


	VkResult result = vkCreateDescriptorPool(logicalDevice, &poolCreateInfo, nullptr, &m_descriptorPool);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create ImGui DescriptorPool");
	}
}

void UI::InitImGui(GLFWwindow* window, VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, u32 queueFamily, VkQueue queue, u32 minImageCount, u32 imageSize, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForVulkan(window, true);

	ImGui_ImplVulkan_InitInfo initInfo = {};
	initInfo.Instance = instance;
	initInfo.PhysicalDevice = physicalDevice;
	initInfo.Device = logicalDevice;
	initInfo.QueueFamily = queueFamily;
	initInfo.Queue = queue;
	initInfo.DescriptorPool = m_descriptorPool;
	initInfo.MinImageCount = minImageCount;
	initInfo.ImageCount = imageSize;
	initInfo.MSAASamples = msaaSamples;
	initInfo.RenderPass = renderPass;

	ImGui_ImplVulkan_Init(&initInfo);

	VkCommandBuffer commandBuffer = BeginSingleTimeCommands(logicalDevice); 
	EndSingleTimeCommands(queue);
}

VkCommandBuffer UI::BeginSingleTimeCommands(VkDevice logicalDevice) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffers);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffers, &beginInfo);

	return commandBuffers;
}

void UI::EndSingleTimeCommands(VkQueue queueSubmit) {
	vkEndCommandBuffer(commandBuffers);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers;

	vkQueueSubmit(queueSubmit, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queueSubmit);
}

void UI::CreateUICommandPool(VkDevice logicalDevice, u32 queueFamilyIndex){
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndex;

	VkResult result = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create UI Command Pool");
	}
	else {
		std::cout << "UI Command Pool Created-> " << commandPool << std::endl;
	}
}


void UI::Cleanup(VkDevice logicalDevice) {
	vkDeviceWaitIdle(logicalDevice);

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffers);
}



void UI::UISettings(LightObject& Rlight) {
	// Set Blender-like style
	ImGui::StyleColorsDark(); // Start with dark theme as base
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	float panel_width = viewport->Size.x * 0.3f;
	float panel_height = viewport->Size.y;

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	// ===== BLENDER COLOR PALETTE =====
	// Backgrounds
	colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f); // Dark gray main background
	colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);  // Slightly darker child bg
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.98f);  // Popup background

	// Interactive elements
	colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f); // Input fields, checkboxes
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

	// Blender's signature blue accents
	const ImVec4 blender_blue(0.27f, 0.51f, 0.71f, 1.00f);
	const ImVec4 blender_blue_hover(0.35f, 0.60f, 0.80f, 1.00f);
	const ImVec4 blender_blue_active(0.20f, 0.45f, 0.65f, 1.00f);

	// Buttons
	colors[ImGuiCol_Button] = blender_blue;
	colors[ImGuiCol_ButtonHovered] = blender_blue_hover;
	colors[ImGuiCol_ButtonActive] = blender_blue_active;

	// Headers and tabs
	colors[ImGuiCol_Header] = blender_blue;
	colors[ImGuiCol_HeaderHovered] = blender_blue_hover;
	colors[ImGuiCol_HeaderActive] = blender_blue_active;

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TabHovered] = blender_blue_hover;
	colors[ImGuiCol_TabActive] = blender_blue;
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

	// Text
	colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f); // Bright text
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

	// Borders and separators
	colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

	// ===== BLENDER-LIKE STYLING =====
	style.WindowPadding = ImVec2(10, 10); // Tighter padding than default
	style.FramePadding = ImVec2(8, 8);  // Compact controls
	style.ItemSpacing = ImVec2(10, 10);   // Tight spacing
	style.ItemInnerSpacing = ImVec2(4, 4);

	style.WindowRounding = 4.0f;
	style.FrameRounding = 3.0f;
	style.PopupRounding = 4.0f;
	style.GrabRounding = 3.0f;
	style.ScrollbarRounding = 4.0f;
	style.TabRounding = 3.0f;

	// Panel 1 (Properties)
	ImGui::SetNextWindowPos(ImVec2(viewport->Size.x - panel_width, 0));
	ImGui::SetNextWindowSize(ImVec2(panel_width, panel_height / 2));
	if (ImGui::Begin("Render Features", nullptr, window_flags)) {
		// Checkbox group
		ImGui::PushStyleColor(ImGuiCol_Text, blender_blue);
		ImGui::Text("RENDER FEATURES");
		ImGui::PopStyleColor();
		ImGui::Separator();

		//Lighting toggles
		ImGui::Spacing();
		static bool enableAmbientDiffuse = false;
		ImGui::PushItemWidth(-1);
		ImGui::Checkbox("Ambient/Diffuse Lighting", &enableAmbientDiffuse);

		static bool enableShadowMapping = false;
		ImGui::Checkbox("Shadow Mapping", &enableShadowMapping);

		static bool enablePointLight = false;
		ImGui::Checkbox("PointLight", &enablePointLight);

		ImGui::Separator();
		ImGui::Text("Light Settings");

		float col[3] = { Rlight.color.r, Rlight.color.g, Rlight.color.b };
		if (ImGui::ColorEdit3("Color", col)) {
			Rlight.color = glm::vec4(col[0], col[1], col[2], 1.0f);
		}

		// Ambient slider
		ImGui::Text("Ambient");
		ImGui::BeginDisabled(!enableAmbientDiffuse);
		ImGui::SliderFloat("Ambient", &Rlight.ambient, 0.0f, 1.0f);
		ImGui::EndDisabled();

		ImGui::PopItemWidth();
	}
	ImGui::End();

	// Panel 2 (Tools)
	ImGui::SetNextWindowPos(ImVec2(viewport->Size.x - panel_width, panel_height / 2));
	ImGui::SetNextWindowSize(ImVec2(panel_width, panel_height / 2));
	if (ImGui::Begin("PHSX", nullptr, window_flags)) {
		ImGui::PushStyleColor(ImGuiCol_Text, blender_blue);
		ImGui::Text("PHYSICS");
		ImGui::PopStyleColor();
		ImGui::Spacing();
		ImGui::Separator();
	}
	ImGui::End();

}
