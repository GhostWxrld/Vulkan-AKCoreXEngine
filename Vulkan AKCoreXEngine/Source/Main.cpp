#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION


#include "../Headers/Renderer.h"


int main() {
	Renderer app;

	try {
		std::thread GfxThread(&Renderer::Run, &app);

		GfxThread.join();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}