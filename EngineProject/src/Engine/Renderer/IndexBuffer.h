#pragma once

#include "glad/glad.h"
#include "Engine/Renderer/BaseBuffer.h"

namespace Engine
{
	class IndexBuffer : public BaseBuffer
	{
	public:

		IndexBuffer();

		void SetData(const uint32_t* inIndices, const size_t inCount, const GLenum inDrawType);

		virtual void Bind() const override;

	public:

		FORCEINLINE size_t GetCount() const { return Count; };

	private:
		size_t Count;
		uint32_t Handle;
	};
}
