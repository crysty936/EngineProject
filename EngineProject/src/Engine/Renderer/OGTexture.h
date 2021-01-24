#pragma once
#include <glad/glad.h>

namespace Engine
{
	class OGTexture
	{
	public:
		OGTexture(const char* TexturePath);

		//void SetParameterI(GLenum Target, GLenum Name, GLint Param) const;

	public:
		inline unsigned int GetHandle()const { return Handle; }


	private:
		unsigned char* Data = nullptr;
		unsigned int Handle;
		int Width, Height, NrChannels;

	};
}

