#pragma once
#include "glad/glad.h"

namespace Engine {

	class VertexArray
	{
	public:
		VertexArray();

		void Bind()const;
		void SetAttribPointer(GLuint Index, GLint Size, GLenum Type, GLboolean Normalized, GLsizei stride, const void* pointer) const;
		void EnableAttribArray(int Index)const;

	public:
		inline int GetHandle()const { return Handle; }


	private:
		unsigned int Handle;

	};

}

