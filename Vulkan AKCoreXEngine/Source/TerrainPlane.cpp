#include "../Headers/TerrainPlane.h"


void TerrainPlane::TerrainGeometry(float terrainScale, int subdivisions) {
	terrainVertices.clear();
	terrainIndices.clear();

	int vertexCount = subdivisions + 1;
	float step = 2.0f * terrainScale / subdivisions;

	std::cout << "=== GENERATING SUBDIVIDED TERRAIN ===" << std::endl;
	std::cout << "Scale: " << terrainScale << std::endl;
	std::cout << "Subdivisions: " << subdivisions << std::endl;
	std::cout << "Expected vertices: " << vertexCount * vertexCount << std::endl;
	std::cout << "Expected indices: " << subdivisions * subdivisions * 6 << std::endl;

	// Create vertices in a grid
	for (int z = 0; z <= subdivisions; z++) {
		for (int x = 0; x <= subdivisions; x++) {
			float xPos = -terrainScale + x * step;
			float zPos = -terrainScale + z * step;

			terrainVertices.push_back({
				{xPos, 0.0f, zPos},
				{0.0f, 1.0f, 0.0f}  // Default normal (up)
				});
		}
	}

	// Create indices for triangles
	for (int z = 0; z < subdivisions; z++) {
		for (int x = 0; x < subdivisions; x++) {
			int topLeft = z * vertexCount + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * vertexCount + x;
			int bottomRight = bottomLeft + 1;

			// First triangle
			terrainIndices.push_back(topLeft);
			terrainIndices.push_back(bottomLeft);
			terrainIndices.push_back(topRight);

			// Second triangle
			terrainIndices.push_back(topRight);
			terrainIndices.push_back(bottomLeft);
			terrainIndices.push_back(bottomRight);
		}
	}

	vertexBufferSize = sizeof(terrainVertices[0]) * terrainVertices.size();
	indexBufferSize = sizeof(terrainIndices[0]) * terrainIndices.size();
	terrainIndexCount = static_cast<u32>(terrainIndices.size());

	std::cout << "Actual vertices generated: " << terrainVertices.size() << std::endl;
	std::cout << "Actual indices generated: " << terrainIndices.size() << std::endl;
	std::cout << "Terrain index count: " << terrainIndexCount << std::endl;
}


TerrainPlane::~TerrainPlane(){

}


