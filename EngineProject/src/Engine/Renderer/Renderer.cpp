#include "pch.h"
#include "Renderer.h"

namespace Engine 
{
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
