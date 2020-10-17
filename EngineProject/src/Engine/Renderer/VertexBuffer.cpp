#include "pch.h"
#include "VertexBuffer.h"

namespace Engine
{
	 VertexBuffer::VertexBuffer() 
		 : BaseBuffer() {}

	void VertexBuffer::SetData(const float* Vertices, const int count, const GLenum DrawType)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, Vertices, DrawType);
	}

	void VertexBuffer::SetData(const void* data, const uint32_t size, const GLenum DrawType)
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, DrawType);
	}
}
