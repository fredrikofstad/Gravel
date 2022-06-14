#pragma once

#include <string>

struct ShaderStrings
{
	std::string Vertex;
	std::string Fragment;
};

namespace Gravel {
	class Shader 
	{
	public:
		Shader(const std::string& filepath);
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		uint32_t CompileShader(const std::string& vertexSource, const std::string& fragmentSource);

		void Bind() const;
		void Unbind() const;


		ShaderStrings ParseShader(const std::string file);

	private:
		uint32_t m_rendererID;
	};
}