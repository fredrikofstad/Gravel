#include "grpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Gravel {
	Shared<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
		case RendererAPI::API::OpenGL: return MakeShared<OpenGLVertexBuffer>(size);
		default: GR_CORE_ASSERT(false, "RendererAPI not implemented in buffer's switch"); return nullptr;
		}
	}
	Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeShared<OpenGLVertexBuffer>(vertices, size);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in buffer's switch"); return nullptr;
		}
	
	}

	Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) 
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeShared<OpenGLIndexBuffer>(indices, count);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in buffer's switch"); return nullptr;
		}

	}

}