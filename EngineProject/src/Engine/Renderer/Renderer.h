#pragma once

#include "glad/glad.h"

namespace Engine 
{
#define GlCall(x) {GLClearError(); x; ENGINE_CORE_ASSERT(GlLogCall(#x, __FILE__, __LINE__),"OpenGl Error")}

	void GLFWErrorCallback(int errorType, const char* errorDesc);
	void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	void GLClearError();
	bool GlLogCall(const char* Function, const char* File, long Line);



	class Renderer
	{








	};
}

