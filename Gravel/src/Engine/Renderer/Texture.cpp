#include "grpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Gravel {

	Shared<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeShared<OpenGLTexture2D>(width, height);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented."); return nullptr;
		}
	}

	Shared<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeShared<OpenGLTexture2D>(filepath);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented."); return nullptr;
		}
	}



}