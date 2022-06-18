#pragma once

#include "Engine/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Gravel {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetUniformInt(const std::string& name, int value);

		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat2(const std::string& name, const glm::vec2& values);
		void SetUniformFloat3(const std::string& name, const glm::vec3& values);
		void SetUniformFloat4(const std::string& name, const glm::vec4& values);

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

		uint32_t CompileShader(const std::string& vertexSource, const std::string& fragmentSource);
		ShaderStrings ParseShader(const std::string file);

	private:
		uint32_t m_rendererID;
	};
}