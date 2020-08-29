#include "pch.h"
#include "OpenGLUtils.h"
#include <string>
#include "Engine/Core/Log.h"
#include "glad/glad.h"
#include "Engine/Core/Log.h"


namespace Engine
{
	static GLenum GlCheckError_(const char* file, int line)
	{
		GLenum errorCode{};

		while ((errorCode != glGetError()) != GL_NO_ERROR)
		{
			std::string error;

			switch (errorCode)
			{
			case GL_INVALID_ENUM:					error = "Invalid Enum"; break;
			case GL_INVALID_VALUE:					error = "Invalid Value"; break;
			case GL_INVALID_OPERATION:				error = "Invalid Operation"; break;
			case GL_STACK_OVERFLOW:					error = "Stack Overflow"; break;
			case GL_STACK_UNDERFLOW:				error = "Stuack Underflow"; break;
			case GL_OUT_OF_MEMORY:					error = "Quack, out of memory"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "Invalid Framebuffer Operation"; break;
			}
			CORE_ERROR("{0} | {1}  ( {2} ) ", error, file, line);
		}
		return errorCode;
	}
}

