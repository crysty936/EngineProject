#include "pch.h"
#include "OpenGLContext.h"
#include <Glad/glad.h>

namespace Engine {


	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window)
	{
		ENGINE_CORE_ASSERT(m_Window, "Window is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_CORE_ASSERT(status, "Failed to Initialize Glad");

		CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}


