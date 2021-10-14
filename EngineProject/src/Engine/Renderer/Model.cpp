#include "pch.h"
#include "Model.h"
#include "Engine/Renderer/Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Engine/Renderer/TextureUtils.h"

namespace Engine
{
	std::vector<Texture> Model::LoadedTextures;

	Model::Model(const char* Path)
	{
		LoadModel(Path);
	}

	Model::Model(std::vector<Mesh>& inMeshes)
		:Meshes(inMeshes)
	{

	}

	void Model::Draw(Shader& InShader)
	{
		for (Mesh& currentMesh : Meshes)
		{
			currentMesh.Draw(InShader);
		}
	}

	void Model::AddMesh(Mesh& InMesh)
	{
		Meshes.push_back(InMesh);
	}

	void Model::AddMeshes(std::vector<Mesh>& InMeshes)
	{
		Meshes.insert(Meshes.end(), InMeshes.begin(), InMeshes.end());
	}

	void Model::LoadModel(const std::string Path)
	{
		Assimp::Importer Importer;
		const aiScene* Scene = Importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
		{
			LOG_CORE_ERROR("Error::Assimp:: {0}", Importer.GetErrorString());
		}

		Directory = Path.substr(0, Path.find_last_of('/'));

		if (Scene)
			ProcessNode(Scene->mRootNode, Scene);
	}

	void Model::ProcessNode(aiNode* Node, const aiScene* Scene)
	{
		for (uint32_t i = 0; i < Node->mNumMeshes; i++)
		{
			aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
			Meshes.push_back(ProcessMesh(Mesh, Scene));
		}

		for (uint32_t i = 0; i < Node->mNumChildren; i++)
		{
			ProcessNode(Node->mChildren[i], Scene);
		}
	}
	Mesh Model::ProcessMesh(aiMesh* InMesh, const aiScene* Scene)
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<Texture> Textures;

		for (uint32_t i = 0; i < InMesh->mNumVertices; i++)
		{
			Vertex V;
			aiVector3D CVertex = InMesh->mVertices[i];
			aiVector3D CNormal = InMesh->mNormals[i];
			V.Position = glm::vec3(CVertex.x, CVertex.y, CVertex.z);
			V.Normal = glm::vec3(CNormal.x, CNormal.y, CNormal.z);

			if (InMesh->mTextureCoords[0])
			{
				aiVector3D CTexCoords = InMesh->mTextureCoords[0][i];
				glm::vec2 Coords;
				Coords.x = CTexCoords.x;
				Coords.y = CTexCoords.y;
				V.TexCoords = Coords;
			}
			else
			{
				V.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			Vertices.push_back(V);
		}

		for (uint32_t i = 0; i < InMesh->mNumFaces; i++)
		{
			aiFace Face = InMesh->mFaces[i];

			for (uint32_t j = 0; j < Face.mNumIndices; j++)
			{
				Indices.push_back(Face.mIndices[j]);
			}
		}

		if (InMesh->mMaterialIndex >= 0)
		{
			aiMaterial* Material = Scene->mMaterials[InMesh->mMaterialIndex];
			std::vector<Texture> DiffuseMaps = LoadMaterialTextures(Material, aiTextureType_DIFFUSE, TextureType::TextureDiffuse);
			Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

			std::vector<Texture> SpecularMaps = LoadMaterialTextures(Material, aiTextureType_SPECULAR, TextureType::TextureSpecular);
			Textures.insert(Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
		}

		return Mesh(Vertices, Indices, Textures);
	}
	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, TextureType TexType)
	{
		std::vector<Texture> Textures;

		for (uint32_t i = 0; i < Mat->GetTextureCount(Type); i++)
		{
			aiString Str;
			Mat->GetTexture(Type, i, &Str);
			Texture Tex;

			if (!IsTextureLoaded(Str.C_Str(), Tex))
			{
				std::string Path = Directory + std::string("/") + std::string(Str.C_Str());
				TextureUtils::LoadTexture(Path.c_str(), Tex.Id);
				Tex.Type = TexType;
				Tex.Path = std::string(Str.C_Str());
				LoadedTextures.push_back(Tex);
			}

			Textures.push_back(Tex);
		}

		return Textures;
	}
	bool Model::IsTextureLoaded(const std::string& TexPath, Texture& OutTex)
	{
		for (const auto& LoadedTexture : LoadedTextures)
		{
			if (LoadedTexture.Path._Equal(TexPath))
			{
				OutTex = LoadedTexture;
				return true;
			}
		}
		return false;
	}
}

