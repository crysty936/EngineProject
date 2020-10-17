#include "pch.h"

#include "glad/glad.h"
#include "BaseBuffer.h"

namespace Engine
{
	BaseBuffer::BaseBuffer()
	{
		glGenBuffers(1, &Handle);
	}

	BaseBuffer::~BaseBuffer()
	{
	}

	void BaseBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, Handle);
	}

	void BaseBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	uint32_t BaseBuffer::GetHandle() const
	{
		return Handle;
	}

}
