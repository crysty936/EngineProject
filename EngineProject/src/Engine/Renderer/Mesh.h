#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Engine
{
	enum class TextureType : uint8_t
	{
		TextureDiffuse,
		TextureSpecular
	};

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture
	{
		unsigned int Id;
		TextureType Type;
		std::string Path;
	};

	class Mesh
	{

	public:

		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;

		Mesh(std::vector<Vertex> InVertices, std::vector<unsigned int> InIndices, std::vector<Texture> InTextures);

		void Draw(class Shader& InShader);

	private:

		class IndexBuffer* EBO;
		class VertexArray* VAO;

		void SetupMesh();

#define TEXTURE_DIFFUSE  "texture_diffuse"
#define TEXTURE_SPECULAR "texture_specular"

	};

}