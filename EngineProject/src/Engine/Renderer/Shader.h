#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Engine
{
	class Shader
	{
	public:

		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void UnBind();
		void SetUniformValue(char* const UniformName, float v1, float v2, float v3, float v4) const;
		void SetUniformValue(char* const UniformName, float v1, float v2, float v3) const;

		uint32_t inline GetHandle() const { return m_RendererID; }


	private:

		uint32_t m_RendererID = 0;
	};

}


