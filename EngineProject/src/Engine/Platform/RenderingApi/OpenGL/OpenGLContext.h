#pragma once

#include "Engine/Renderer/IRenderingContext.h"
#include <GLFW/glfw3.h>

namespace Engine {


	class OpenGLContext : public IRenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
}


