#pragma once

#include "glad/glad.h"
#include "Engine/Renderer/BaseBuffer.h"

namespace Engine
{
	class VertexBuffer : public BaseBuffer
	{
	public:
		VertexBuffer();

	public:
		void SetData(const float* Vertices, const int count, const uint32_t DrawType);
		void SetData(const void* data, const uint32_t size, const uint32_t DrawType);
	};

}
