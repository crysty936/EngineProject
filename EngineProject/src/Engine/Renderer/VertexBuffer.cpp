#include "pch.h"
#include "VertexBuffer.h"
#include <glad/glad.h>

namespace Engine
{

	VertexBuffer::VertexBuffer(unsigned int _BufferType)
		:Type(_BufferType)
	{
		glGenBuffers(1, &Handle);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(Type, Handle);
	}

	void VertexBuffer::SetData(const float* Vertices, const int size, const unsigned int DrawType)
	{
		glBufferData(Type, size, Vertices, DrawType);
	}
}
