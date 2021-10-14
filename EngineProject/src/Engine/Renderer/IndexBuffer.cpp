#include "pch.h"
#include "IndexBuffer.h"

namespace Engine
{
	IndexBuffer::IndexBuffer()
		:BaseBuffer(), Count(0)  {}

	void IndexBuffer::SetData(const uint32_t* inIndices, const size_t inCount, const GLenum inDrawType)
	{
		const GLsizei count = static_cast<GLsizei>(inCount);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, inIndices, inDrawType);
		Count = inCount;
	}
	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetHandle());
	}
}