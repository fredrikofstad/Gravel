#include "grpch.h"

#include "OpenGLShader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>


namespace Gravel {

	static GLenum ShaderType(const std::string& type)
	{
		if (type == "vertex")   return GL_VERTEX_SHADER;
		if (type == "fragment") return GL_FRAGMENT_SHADER;
		GR_CORE_ASSERT(false, "Unsupported shader type");
		return 0;
	}
	
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = MakeFile(filepath);
		auto shaderSources = ParseShader(source);
		CompileShader(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;

		CompileShader(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererID);
	}

	void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& shaderSources)
	{

		GLuint program = glCreateProgram();
		std::vector<GLenum> shaderIDs(shaderSources.size());

		for (auto && [type, source] : shaderSources)
		{
			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceString = source.c_str();
			glShaderSource(shader, 1, &sourceString, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;

			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				GR_CORE_ERROR("{0}", infoLog.data());
				GR_CORE_ASSERT(false, "Shader failed to compile.");
				return;
			}

			// attach if successfully compiled and add id to vector
			glAttachShader(program, shader);
			shaderIDs.push_back(shader);
		}

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
			for (auto id : shaderIDs) 
				glDeleteShader(id);

			GR_CORE_ERROR("{0}", infoLog.data());
			GR_CORE_ASSERT(false, "Shader link failure.");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : shaderIDs)
			glDetachShader(program, id);

		// set member var if compilation was a success
		m_rendererID = program;
	}


	void OpenGLShader::Bind() const
	{
		glUseProgram(m_rendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}


	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::MakeFile(const std::string filepath)

	{
		std::string string;
		std::ifstream stream(filepath, std::ios::in, std::ios::binary);

		if (!stream) GR_CORE_ERROR("Couldn't read file {0}.", filepath);

		stream.seekg(0, std::ios::end);
		string.resize(stream.tellg());
		stream.seekg(0, std::ios::beg);
		stream.read(&string[0], string.size());
		stream.close();

		return string;
	}
	
	std::unordered_map<GLenum, std::string> OpenGLShader::ParseShader(const std::string file)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeKey = "#type";
		size_t typeKeyLength = strlen(typeKey);
		size_t position = file.find(typeKey, 0);
		while (position != std::string::npos)
		{
			size_t endOfLine = file.find_first_of("\r\n", position);
			GR_CORE_ASSERT(endOfLine != std::string::npos, "Syntax Error.");

			size_t begin = position + typeKeyLength + 1;
			std::string type = file.substr(begin, endOfLine - begin);
			GR_CORE_ASSERT(ShaderType(type), "Shader type not supported.");

			size_t nextLinePos = file.find_first_of("\r\n", endOfLine);
			position = file.find(typeKey, nextLinePos);
			shaderSources[ShaderType(type)] = file.substr(nextLinePos, 
				position - (nextLinePos == std::string::npos ? file.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

}
