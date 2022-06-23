#pragma once

#include "Engine/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO: FIX this shit
typedef unsigned int GLenum;

namespace Gravel {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& floats) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& floats) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

		virtual const std::string& GetName() const override { return m_name; }
		void SetUniformInt(const std::string& name, int value);
		void SetUniformIntArray(const std::string& name, int* values, uint32_t count);

		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat2(const std::string& name, const glm::vec2& values);
		void SetUniformFloat3(const std::string& name, const glm::vec3& values);
		void SetUniformFloat4(const std::string& name, const glm::vec4& values);

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

		void CompileShader(const std::unordered_map<GLenum, std::string>& shaderSources);

		std::unordered_map<GLenum, std::string> ParseShader(const std::string file);
		std::string MakeFile(const std::string filepath);

	private:
		uint32_t m_rendererID;
		std::string m_name;
	};
}