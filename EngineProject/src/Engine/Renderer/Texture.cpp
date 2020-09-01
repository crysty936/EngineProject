#include "pch.h"
#include "Texture.h"
#include "stb_image.h"

namespace Engine
{
	Texture::Texture(const char* TexturePath, int ImageType)
	{
		glGenTextures(1, &Handle);
		glBindTexture(GL_TEXTURE_2D, Handle);

		unsigned char* data = stbi_load(TexturePath, &Width, &Height, &NrChannels, 0);

		stbi_set_flip_vertically_on_load(true);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, ImageType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

// 	void Texture::SetParameterI(GLenum Target, GLenum Name, GLint Param) const
// 	{
// 		glTexParameteri(Target, Name, Param);
// 	}

}
