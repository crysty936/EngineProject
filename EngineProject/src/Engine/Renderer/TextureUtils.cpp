#include "pch.h"
#include "TextureUtils.h"
#include "stb_image.h"

namespace Engine
{
	void TextureUtils::LoadTexture(const char* TexturePath, uint32_t& OutHandle)
	{
		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &OutHandle);
		glBindTexture(GL_TEXTURE_2D, OutHandle);

		int Width, Height, NrChannels;
		unsigned char* data = stbi_load(TexturePath, &Width, &Height, &NrChannels, 0);

		GLenum Format = 0;
		switch (NrChannels)
		{
		case 1:
		{
			Format = GL_RED;
			break;
		}
		case 3:
		{
			Format = GL_RGB;
			break;
		}
		case 4:
		{
			Format = GL_RGBA;
			break;
		}

		}

		ENGINE_ASSERT(data, "Image Loading Failed");

		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}

}
