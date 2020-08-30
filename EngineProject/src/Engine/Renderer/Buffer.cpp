#include "pch.h"
#include "Buffer.h"
#include <glad/glad.h>

namespace Engine
{

	Buffer::Buffer(unsigned int _BufferType)
		:Type(_BufferType)
	{
		glGenBuffers(1, &Handle);
	}

	void Buffer::Bind() const
	{
		glBindBuffer(Type, Handle);
	}

	void Buffer::SetData(const float* Vertices, const int size, const unsigned int DrawType)
	{
		glBufferData(Type, size, Vertices, DrawType);
	}
	void Buffer::SetData(const unsigned int* Indices, const int size, const unsigned int DrawType)
	{
		glBufferData(Type, size, Indices, DrawType);
	}
}
