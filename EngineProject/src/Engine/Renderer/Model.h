#pragma once

#include "Engine/Renderer/Mesh.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

namespace Engine
{
	class Model
	{

	public:
		Model(char* Path);
		void Draw(class Shader& InShader);

	private:
		std::vector<Mesh> Meshes;
		std::string Directory;

	private:
		void LoadModel(std::string Path);
		void ProcessNode(aiNode* Node, const aiScene* Scene);
		Mesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, TextureType TexType);

	private:
		static std::vector<Texture> LoadedTextures;
		static bool IsTextureLoaded(const std::string& TexPath, Texture& OutTex);
	};

}

