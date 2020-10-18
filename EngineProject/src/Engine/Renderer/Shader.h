#pragma once

#include <string>
#include <Glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>


namespace Engine
{

	class Shader
	{
	public:

		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		Shader(const char* vertexSrc, const char* fragmentSrc);
		~Shader();

		void Bind();
		void UnBind();
		void SetUniformValue4f(const std::string& UniformName, float v1, float v2, float v3, float v4);
		void SetUniformValue3f(const std::string& UniformName, float v1, float v2, float v3);
		void SetUniformValue1f(const std::string& UniformName, float v1);
		void SetUniformValue1i(const std::string& UniformName, int v1);
		void SetUniformValue4fv(const std::string& UniformName, glm::mat4 matrix);
		void SetUniformValue3fv(const std::string& UniformName, glm::vec3 vec);



		uint32_t inline GetHandle() const { return m_RendererID; }

	private:

		int GetUniformLocation(const std::string& UniformName);
		unsigned int CreateShader(const std::string& Source, GLenum ShaderType);
		std::unordered_map<std::string, int> UniformLocations;


	private:

		uint32_t m_RendererID = 0;
	};

}


