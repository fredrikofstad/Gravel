#include "grpch.h"

#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>


namespace Gravel {

	Shader::Shader(const std::string& filepath)
	{
		ShaderStrings shaderStrings = ParseShader(filepath);
		m_rendererID = CompileShader(shaderStrings.Vertex, shaderStrings.Fragment);
	}

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_rendererID = CompileShader(vertexSource, fragmentSource);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_rendererID);
	}

	uint32_t Shader::CompileShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			GR_CORE_ERROR("{0}", infoLog.data());
			GR_CORE_ASSERT(false, "Vertex shader failed to compile.");
			return 0;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			GR_CORE_ERROR("{0}", infoLog.data());
			GR_CORE_ASSERT(false, "Fragment shader failed to compile.");
			return 0;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();

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
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			GR_CORE_ERROR("{0}", infoLog.data());
			GR_CORE_ASSERT(false, "Shader link failure.");
			return 0;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		return program;
	}


	void Shader::Bind() const
	{
		glUseProgram(m_rendererID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	ShaderStrings Shader::ParseShader(const std::string file)
	{
		std::ifstream stream(file);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		ShaderType type = ShaderType::NONE;

		std::string line;
		std::stringstream ss[2];
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}
		return { ss[0].str(), ss[1].str() };
	}

}
