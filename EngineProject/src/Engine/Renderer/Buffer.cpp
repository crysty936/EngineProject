#include "pch.h"
#include "Buffer.h"
#include <glad/glad.h>

namespace Engine
{

	Buffer::Buffer(uint32_t _BufferType)
		:Type(_BufferType)
	{
		glGenBuffers(1, &Handle);
	}

	void Buffer::Bind() const
	{
		glBindBuffer(Type, Handle);
	}

	void Buffer::SetData(const float* Vertices, const int count, const uint32_t DrawType)
	{
		glBufferData(Type, sizeof(float) * count, Vertices, DrawType);
		Count = count;
	}
	void Buffer::SetData(const uint32_t* Indices, const int count, const uint32_t DrawType)
	{
		glBufferData(Type, sizeof(uint32_t) * count, Indices, DrawType);
		Count = count;
	}
}
