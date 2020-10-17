#pragma once

#include <vector>
#include "glad/glad.h"
#include "Engine/Core/Core.h"


namespace Engine
{
	struct LayoutProperties
	{
		uint32_t Type;
		uint32_t Count;
		uint32_t Normalized;


		static uint32_t GetSizeOfType(uint32_t Type)
		{
			switch (Type)
			{
			case GL_FLOAT:			return sizeof(float);
			case GL_UNSIGNED_INT:	return sizeof(unsigned int);
			}

			ENGINE_CORE_ASSERT(false, "");
			return 0;
		}

	};


	class VertexBufferLayout
	{

	public:

		VertexBufferLayout()
			:Stride(0) {}


		template<typename T>
		void Push(uint32_t Count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(uint32_t Count)
		{
			Properties.push_back({ GL_FLOAT, Count, GL_FALSE });
			Stride += Count * sizeof(float);
		}

		template<>
		void Push<uint32_t>(uint32_t Count)
		{
			Properties.push_back({ GL_UNSIGNED_INT, Count, GL_FALSE });
			Stride += Count * sizeof(uint32_t);
		}

		FORCEINLINE const std::vector<LayoutProperties>& GetProperties() const { return Properties; }
		FORCEINLINE const uint32_t GetStride() const { return Stride; }

	private:
		std::vector<LayoutProperties> Properties;
		uint32_t Stride;


	};


}
