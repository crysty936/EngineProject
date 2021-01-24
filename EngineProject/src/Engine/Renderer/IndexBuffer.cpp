#include "pch.h"
#include "IndexBuffer.h"

namespace Engine
{
	IndexBuffer::IndexBuffer()
		:BaseBuffer(), Count(0)  {}

	void IndexBuffer::SetData(const uint32_t* Indices, const int _Count, const GLenum DrawType)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * _Count, Indices, DrawType);
		Count = _Count;
	}
	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetHandle());
	}
}