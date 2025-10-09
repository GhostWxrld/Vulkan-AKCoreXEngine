#pragma once

#define M_PI 3.14159265358979323846
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.h>
#include <vector> 

#include "Vertices.h"

class Blackhole {
public:

	struct BHPC {
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 cameraPos;

		glm::vec3 bhPosition;
		float bhRadius;
	} m_pushConstant;

	Blackhole() = default;
	~Blackhole() {};

	void GenerateSphere(std::vector<BlackholeVertex>& vertices, std::vector<u32>& indices, float radius, u32 sectorCount, u32 stackCount) {
		vertices.clear();
		indices.clear();

		float x, y, z, xy;
		float nx, ny, nz;
		float sectorStep = 2 * M_PI / sectorCount;
		float stackStep = M_PI / stackCount;
		float sectorAngle, stackAngle;

		for (uint32_t i = 0; i <= stackCount; ++i) {
			stackAngle = M_PI / 2 - i * stackStep; // from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);

			for (uint32_t j = 0; j <= sectorCount; ++j) {
				sectorAngle = j * sectorStep;
				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);
				nx = x / radius;
				ny = y / radius;
				nz = z / radius;
				vertices.push_back({ glm::vec3(x, y, z), glm::vec3(nx, ny, nz) });
			}
		}

		// indices
		uint32_t k1, k2;
		for (uint32_t i = 0; i < stackCount; ++i) {
			k1 = i * (sectorCount + 1);
			k2 = k1 + sectorCount + 1;

			for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2) {
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				if (i != (stackCount - 1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}

};
