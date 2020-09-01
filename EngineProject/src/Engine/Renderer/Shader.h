#pragma once

#include <string>
#include <Glad/glad.h>

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
		void SetUniformValue(char* const UniformName, float v1, float v2, float v3, float v4) const;
		void SetUniformValue(char* const UniformName, float v1, float v2, float v3) const;
		void SetUniformValue(char* const UniformName, float v1) const;
		void SetUniformValue(char* const UniformName, int v1) const;

		uint32_t inline GetHandle() const { return m_RendererID; }

	private:

		unsigned int CreateShader(const std::string& Source, GLenum ShaderType);


	private:

		uint32_t m_RendererID = 0;
	};

}


