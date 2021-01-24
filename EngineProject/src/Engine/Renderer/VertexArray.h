#pragma once
#include "glad/glad.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Engine 
{

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind()const;
		void AddBuffer(const VertexBuffer& VBuffer, const VertexBufferLayout& Layout);

	public:
		static void Unbind();

	public:
		inline int GetHandle()const { return Handle; }


	private:
		unsigned int Handle;

	};

}

