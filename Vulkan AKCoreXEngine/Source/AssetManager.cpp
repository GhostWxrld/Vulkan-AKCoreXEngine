#include "../Headers/AssetManager.h"

void AssetManager::LoadModel(const std::string& modelPath){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, 
											aiProcess_Triangulate | 
											aiProcess_FlipUVs | 
											aiProcess_GenSmoothNormals |
											aiProcess_CalcTangentSpace);

	if (!scene || scene->mRootNode) {
		throw std::runtime_error("Failed to load model: " + modelPath);
	}

	//ProcessNodes(scene->mRootNode, scene);
}

void AssetManager::LoadTexture(const std::string& texturePath){

}

/*void AssetManager::ProcessNodes(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNodes(node->mChildren[i], scene);
	}
}

void AssetManager::ProcessMesh(aiMesh* mesh, const aiScene* scene){
	std::vector<Vertex> vertices;
	std::vector<u32> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex{};

		vertex.pos = {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		};
		
		if (mesh->HasNormals()) {
			vertex.normal = {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};
		}

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord = {
				mesh->mTextureCoords[0][i].x,
				1.0f - mesh->mTextureCoords[0][i].y
			};
		}
		else {
			vertex.texCoord = { 0.0f, 0.0f };								//Default tex coords if not availble 
		}

		vertex.color = { 1.0f, 1.0f, 1.0f };
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh outMesh{};
	CreateBuffer(vertices, indices, outMesh);
	return outMesh;
}


//Fix this function, it is confusing and not fully implemented. 
//Don't use Create buffer from Renderer.cpp, use the one from AssetManager.cpp (Create One)


void AssetManager::CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, Mesh& mesh){
	VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();
	VkDeviceSize indexSize = sizeof(indices[0]) * indices.size();

} */


