#include "pch.h"
#include "OGTexture.h"
#include "stb_image.h"

namespace Engine
{
	OGTexture::OGTexture(const char* TexturePath)
	{
		stbi_set_flip_vertically_on_load(true);

 		glGenTextures(1, &Handle);
 		glBindTexture(GL_TEXTURE_2D, Handle);
 
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

// 	void Texture::SetParameterI(GLenum Target, GLenum Name, GLint Param) const
// 	{
// 		glTexParameteri(Target, Name, Param);
// 	}

}
