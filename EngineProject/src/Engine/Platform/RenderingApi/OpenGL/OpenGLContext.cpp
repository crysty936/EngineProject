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
		LOG_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));

// 		glEnable(GL_CULL_FACE);
// 		glCullFace(GL_BACK);
// 		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);


		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}


