#include "pch.h"
#include "VertexBuffer.h"
#include <glad/glad.h>

namespace Engine
{
	
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &Handle);
	}
	
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, Handle);
	}
	
	void VertexBuffer::SetData(const float* Vertices, const int size, const unsigned int DrawType)
	{
		glBufferData(GL_ARRAY_BUFFER, size, Vertices, DrawType);
	}
}
