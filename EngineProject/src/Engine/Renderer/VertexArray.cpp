#include "pch.h"
#include "VertexArray.h"

namespace Engine
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &Handle);
	}
	
	void VertexArray::Bind() const
	{
		glBindVertexArray(Handle);
	}
	
	void VertexArray::SetAttribPointer(GLuint Index, GLint Size, GLenum Type, GLboolean Normalized, GLsizei stride, const void* pointer) const
	{
		glVertexAttribPointer(Index, Size, Type, Normalized, stride, pointer);
	}
	
	void VertexArray::EnableAttribArray(int Index) const
	{
		glEnableVertexAttribArray(Index);
	}
}
