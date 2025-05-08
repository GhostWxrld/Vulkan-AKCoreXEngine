#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

//TODO: Rotate Camera 45 to match model matrix
//TODO: Disable cursor when in program when controlling camera

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