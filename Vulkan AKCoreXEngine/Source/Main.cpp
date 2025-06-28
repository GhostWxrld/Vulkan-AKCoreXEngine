#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

//TODO: Already set up descriptor pool flags in render to VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT for imgui
//Implement the imgui class to match and work in renderer.

#include "../Headers/Renderer.h"

int main() {
	Renderer app;

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}