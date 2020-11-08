#include "pch.h"
#include "OpenGLUtils.h"
#include <string>
#include "Engine/Core/Log.h"
#include "glad/glad.h"
#include "Engine/Core/Log.h"


namespace Engine
{
	GLenum OpenGLUtils::GlCheckError_(const char* file, int line)
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
			LOG_CORE_ERROR("{0} | {1}  ( {2} ) ", error, file, line);
		}
		return errorCode;
	}

	void GLFWErrorCallback(int errorType, const char* errorDesc)
	{
		LOG_CORE_CRITICAL("GLFW Error Code: {0} , {1}", errorType, errorDesc);
	}

	void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (severity != GL_DEBUG_SEVERITY_HIGH)
		{
			LOG_CORE_WARN("OPENGL Error : {0}", message);
		}
		else
		{
			ENGINE_CORE_ASSERT(0, message);
		}
	}


	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GlLogCall(const char* Function, const char* File, long Line)
	{
		bool error = true;
		while (GLenum error = glGetError())
		{
			LOG_CORE_CRITICAL("OpenGlError at line {0} in {1} calling {2}", Line, File, Function);
			error = false;
		}
		return error;
	}

}

