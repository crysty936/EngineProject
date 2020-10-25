#pragma once

#include "glad/glad.h"

namespace Engine
{
	class Renderer
	{

	private:
		static Renderer* s_Instance;
		Renderer();
		~Renderer();

	public:

		void Draw(const class VertexArray& VA, const class IndexBuffer& IB, const class Shader& S) const;
		void DrawArrays(const class VertexArray& VA, const int Start, const int Count, const class Shader& S) const;

		void Clear() const;

		FORCEINLINE static Renderer& GetRenderer() { return *s_Instance; }
	};
}

