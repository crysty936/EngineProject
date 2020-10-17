#pragma once

#include "glad/glad.h"
#include "Engine/Renderer/BaseBuffer.h"

namespace Engine
{
	class IndexBuffer : public BaseBuffer
	{
	public:
		IndexBuffer();

		void SetData(const uint32_t* Indices, const int count, const GLenum DrawType);

	private:
		uint32_t Count;
	};
}
