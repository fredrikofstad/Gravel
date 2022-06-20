#include "grpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Gravel {

	Unique<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeUnique<OpenGLVertexBuffer>(vertices, size);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in buffer's switch"); return nullptr;
		}
	
	}

	Unique<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) 
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeUnique<OpenGLIndexBuffer>(indices, count);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in buffer's switch"); return nullptr;
		}

	}

}