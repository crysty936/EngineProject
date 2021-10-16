#include "pch.h"

#include "Mesh.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{

	Mesh::Mesh(std::vector<Vertex> InVertices, std::vector<unsigned int> InIndices, std::vector<Texture> InTextures)
	{
		Vertices = InVertices;
		Indices = InIndices;
		Textures = InTextures;

		SetupMesh();
	}

	void Mesh::Draw(Shader& InShader)
	{
		uint32_t DiffuseNr = 1;
		uint32_t SpecularNr = 1;

// 		for (uint32_t i = 0; i < Textures.size(); ++i)
// 		{
// 			glActiveTexture(GL_TEXTURE0 + i);
// 			std::string Number;
// 			switch (Textures[i].Type)
// 			{
// 			case TextureType::TextureDiffuse:
// 			{
// 				Number = std::to_string(DiffuseNr++);
// 				InShader.SetUniformValue1i(("UMaterial." + std::string(TEXTURE_DIFFUSE) + Number).c_str(), i);
// 				break;
// 			}
// 			case TextureType::TextureSpecular:
// 			{
// 				Number = std::to_string(SpecularNr++);
// 				InShader.SetUniformValue1i(("UMaterial." + std::string(TEXTURE_SPECULAR) + Number).c_str(), i);
// 				break;
// 			}
// 			}
// 			glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
// 		}
// 		glActiveTexture(GL_TEXTURE0);

		VAO->Bind();
		EBO->Bind();
		GLsizei count = static_cast<GLsizei>(Indices.size());
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
		VAO->Unbind();
	}

	void Mesh::SetupMesh()
	{
		EBO = new IndexBuffer();
		EBO->Bind();
		const GLsizei indicesCount = static_cast<GLsizei>(Indices.size());
		EBO->SetData(Indices.data(), indicesCount, GL_STATIC_DRAW);

		VAO = new VertexArray();
		VAO->Bind();

		VertexBuffer VBuffer;
		VBuffer.Bind();
		const GLsizei verticesCount = static_cast<GLsizei>(Vertices.size());
		VBuffer.SetData(Vertices.data(), verticesCount * sizeof(Vertex), GL_STATIC_DRAW);

		VertexBufferLayout Layout;
		Layout.Push<float>(3);
		Layout.Push<float>(3);
		Layout.Push<float>(2);

		VAO->AddBuffer(VBuffer, Layout);

		EBO->Bind();

	}
}

