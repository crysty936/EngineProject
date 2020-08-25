#include "pch.h"
#include "VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &Handle);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
}

void VertexBuffer::SetData(const float* Vertices, const int size, const  int DrawType)
{
	glBufferData(GL_ARRAY_BUFFER, size, Vertices, DrawType);
}
