#include "grpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Gravel {

	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSource, fragmentSource);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in Shader's switch"); return nullptr;
		}

	}
}
