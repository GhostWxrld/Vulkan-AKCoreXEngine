#pragma once

#include <vulkan/vulkan.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "assimp/postprocess.h"

#include <string>
#include <vector>
#include <iostream>

#include "Vertices.h"

typedef uint32_t u32;

class AssetManager{
public:

	AssetManager() = default;

	~AssetManager();

	void LoadModel(const std::string& modelPath);
	void LoadTexture(const std::string& texturePath);

	void Draw(VkCommandBuffer commandBuffer);

private:
	struct Mesh {
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		u32 indexCount;
	};

	void ProcessNodes(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, Mesh& mesh);

	std::vector<Mesh> meshes;
};

