#include "pch.h"
#include "Shader.h"
#include <Glad/glad.h>

namespace Engine {

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, nullptr);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			int bytesWritten;
			char* infoLog = (char*)_malloca(maxLength * sizeof(char));
			glGetShaderInfoLog(vertexShader, maxLength, &bytesWritten, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			CORE_ERROR("{0}", infoLog);
			ENGINE_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, nullptr);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = (char*)_malloca(maxLength * sizeof(char));
			int bytesWritten;
			glGetShaderInfoLog(fragmentShader, maxLength, &bytesWritten, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			CORE_ERROR("{0}", infoLog);
			ENGINE_CORE_ASSERT(false, "Fragment shader compilation failure!");

			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = (char*)_malloca(maxLength * sizeof(char));
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CORE_ERROR("{0}", infoLog);
			ENGINE_CORE_ASSERT(false, "Shader link failure!");

			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}

	void Shader::SetUniformValue(char* const UniformName, float v1, float v2, float v3, float v4) const
	{
		int vertexOurColorLocation = glGetUniformLocation(GetHandle(), UniformName);
		ENGINE_CORE_ASSERT(vertexOurColorLocation != -1, "Uniform location could not be found!");
		glUniform4f(vertexOurColorLocation, v1, v2, v3, v4);
	}
	void Shader::SetUniformValue(char* const UniformName, float v1, float v2, float v3) const
	{
		int vertexOurColorLocation = glGetUniformLocation(GetHandle(), UniformName);
		ENGINE_CORE_ASSERT(vertexOurColorLocation != -1, "Uniform location could not be found!");
		glUniform3f(vertexOurColorLocation, v1, v2, v3);
	}

}

