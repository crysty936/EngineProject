#pragma once
#include "Engine/Platform/Window/Window.h"
#include <GLFW/glfw3.h>
#include "Engine/EventsManager/Events/KeyEvents.h"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include "Engine/Renderer/RenderObject.h"
#include <glm/glm.hpp>


struct GLFWwindow;

namespace Engine {

	class Camera;
	class Model;
	class Shader;
	class IRenderingContext;

	class WindowsWindow : public Engine::Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void Close() override;

		void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }

		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		virtual void SetVSync(bool enabled) override;

		virtual bool IsVSync() const override { return m_Data.VSync; }

		virtual void* GetNativeWindow() const override { return m_Window; }

		virtual void Init() override;

		void DoOpenGlStuff();
		void ProcessInput();

		void Draw();
		bool did = false;
		void AddImGuiSlider(const std::string& Name, float* Pointer);

		virtual bool IsRunning() const override { return !glfwWindowShouldClose(m_Window); }
		virtual void SetRunning(bool value) override { glfwSetWindowShouldClose(m_Window, !value); }
	private:
		void SetGlfwCallbacks();

	private:

		unsigned int m_indexBuffer;
		unsigned int m_Texture;
		unsigned int m_TextureSpecular;
		unsigned int m_TextureEmission;
		float v_TheAlpha = 0.f;
		std::vector<RenderObject> RenderObjects;
		RenderObject* LightRO;

		glm::vec3 CameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 EarthPosition = glm::vec3(2.f, 0.0f, 0.0f);
		glm::vec3 MoonPosition = glm::vec3(1.0f, 0.0f, 0.0f);

		glm::vec3 MainPosition = glm::vec3(0.0f, 0.0f, 0.0f);

		std::unique_ptr<Camera> MainCamera;
		std::unique_ptr<IRenderingContext> m_RenderingContext;
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0;
			unsigned int Height = 0;
			bool VSync = false;
		};

		WindowData m_Data;
		Model* TheModel;
		Shader* ModelShader;
		glm::vec3 ModelPosition = glm::vec3(2.0f, 3.0f, 4.0f);

	};
}

