#include "pch.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Engine 
{
	Renderer* Renderer::s_Instance;

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::Draw(const VertexArray& VA, const IndexBuffer& IB, const Shader& S) const
	{
		S.Bind();
		VA.Bind();
		IB.Bind();

		glDrawElements(GL_TRIANGLES, IB.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawArrays(const class VertexArray& VA, const int Start, const int Count, const class Shader& S) const
	{
		S.Bind();
		VA.Bind();

		glDrawArrays(GL_TRIANGLES, Start, Count);
	}

	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

