#pragma once


#include <vector> 
#include <iostream>

#include "Vertices.h"

typedef uint32_t u32;

class TerrainPlane{
public:

	struct TerrainPC {
		glm::vec3 pos;
		glm::vec4 color;
		glm::mat4 view;
		glm::mat4 proj;
	}TPC;

	TerrainPlane() = default; 

	void TerrainGeometry(float terrainScale, int subdivisions);

	VkDeviceSize GetVertexBufferSize() { return vertexBufferSize; }
	VkDeviceSize GetIndexBufferSize() { return indexBufferSize; }
	u32 GetTerrainIndexCount() { return terrainIndexCount; }

	const TerrainStruct* GetTerrainVertexData() { return terrainVertices.data(); }
	const u32* GetTerrainIndexData() { return terrainIndices.data(); }


	~TerrainPlane();

private:
	std::vector<TerrainStruct> terrainVertices;
	std::vector<u32> terrainIndices;

	VkDeviceSize vertexBufferSize;
	VkDeviceSize indexBufferSize;

	u32 terrainIndexCount;

};

