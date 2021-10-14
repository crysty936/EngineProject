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
		Model(const char* Path);
		Model() = default;
		Model(std::vector<Mesh>& inMeshes);
		void Draw(class Shader& InShader);

	public:
		void AddMesh(Mesh& InMesh);
		void AddMeshes(std::vector<Mesh>& InMeshes);
		FORCEINLINE void SetDirectory(std::string& InDirectory) { Directory = InDirectory; }
		FORCEINLINE void SetName(std::string& InName) { Name = InName; }

	private:
		std::vector<Mesh> Meshes;
		std::string Name;
		std::string Directory;

	private:
		void LoadModel(const std::string Path);
		void ProcessNode(aiNode* Node, const aiScene* Scene);
		Mesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, TextureType TexType);

	private:
		static std::vector<Texture> LoadedTextures;
		static bool IsTextureLoaded(const std::string& TexPath, Texture& OutTex);
	};

}

