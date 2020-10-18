#include "pch.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = CreateShader(vertexSrc, GL_VERTEX_SHADER);


		// Create an empty fragment shader handle
		GLuint fragmentShader = CreateShader(fragmentSrc, GL_FRAGMENT_SHADER);

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		m_RendererID = program;

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

			LOG_CORE_ERROR("{0}", infoLog);
			ENGINE_CORE_ASSERT(false, "Shader link failure!");

			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	unsigned int Shader::CreateShader(const std::string& Source, GLenum ShaderType)
	{
		GLuint shaderHandle = glCreateShader(ShaderType);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = Source.c_str();
		glShaderSource(shaderHandle, 1, &source, nullptr);

		// Compile the vertex shader
		glCompileShader(shaderHandle);

		GLint isCompiled = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			int bytesWritten;
			char* infoLog = (char*)_malloca(maxLength * sizeof(char));
			glGetShaderInfoLog(shaderHandle, maxLength, &bytesWritten, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(shaderHandle);

			LOG_CORE_ERROR("{0}", infoLog);
			ENGINE_CORE_ASSERT(false, "Shader compilation failure!");
			return -1;
		}

		return shaderHandle;
	}
	Shader::Shader(const char* VertexPath, const char* FragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexFile;
		std::ifstream fragmentFile;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexFile.open(VertexPath);
			fragmentFile.open(FragmentPath);

			std::stringstream vertexStream;
			std::stringstream fragmentStream;

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertexCode = vertexStream.str();
			fragmentCode = fragmentStream.str();
		}
		catch (const std::ifstream::failure	e)
		{
			LOG_CORE_ERROR("Shader file cannot be read.");
			ENGINE_CORE_ASSERT(false, "Shader read failure.");
		}

		// Create an empty vertex shader handle
		GLuint vertexShader = CreateShader(vertexCode, GL_VERTEX_SHADER);


		// Create an empty fragment shader handle
		GLuint fragmentShader = CreateShader(fragmentCode, GL_FRAGMENT_SHADER);

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		m_RendererID = program;

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

			LOG_CORE_ERROR("{0}", infoLog);
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

	void Shader::SetUniformValue(const std::string& UniformName, float v1, float v2, float v3, float v4)
	{
		glUniform4f(GetUniformLocation(UniformName), v1, v2, v3, v4);
	}
	void Shader::SetUniformValue(const std::string& UniformName, float v1, float v2, float v3)
	{
		glUniform3f(GetUniformLocation(UniformName), v1, v2, v3);
	}

	void Shader::SetUniformValue(const std::string& UniformName, float v1)
	{
		glUniform1f(GetUniformLocation(UniformName), v1);
	}

	void Shader::SetUniformValue(const std::string& UniformName, int v1)
	{
		glUniform1i(GetUniformLocation(UniformName), v1);
	}

	void Shader::SetUniformValue(const std::string& UniformName, glm::mat4 matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(UniformName), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::SetUniformValue(const std::string& UniformName, glm::vec3 vec)
	{
		glUniform3fv(GetUniformLocation(UniformName), 1, glm::value_ptr(vec));
	}

	int Shader::GetUniformLocation(const std::string& UniformName)
	{
		if (UniformLocations.find(UniformName) != UniformLocations.end())
			return UniformLocations[UniformName];

		int uniformLocation = glGetUniformLocation(GetHandle(), UniformName.c_str());
		if (uniformLocation != -1)
			UniformLocations[UniformName] = uniformLocation;

		ENGINE_CORE_ASSERT(uniformLocation != -1, "Uniform location could not be found!");

		return uniformLocation;
	}

}

