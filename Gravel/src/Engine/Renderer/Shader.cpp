#include "grpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Gravel {

	Shared<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeShared<OpenGLShader>(name, vertexSource, fragmentSource);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in Shader's switch"); return nullptr;
		}

	}

	Shared<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeShared<OpenGLShader>(filepath);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in Shader's switch"); return nullptr;
		}

	}

	void ShaderLibrary::Add(const std::string& name, const Shared<Shader>& shader)
	{
		GR_CORE_ASSERT(Exists(name), "Shader already exits.");
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Shared<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}


	Shared<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Shared<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Shared<Shader> ShaderLibrary::Get(const std::string& name)
	{
		GR_CORE_ASSERT(!Exists(name), "Shader doesn't exist!");
		return m_shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_shaders.find(name) == m_shaders.end();
	}
}
