#pragma once
#include <glad/glad.h>

namespace Engine
{
	class TextureUtils
	{
	public:
		static void LoadTexture(const char* TexturePath, uint32_t& OutId);

	};
}

