#include "grpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Gravel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		GR_CORE_ASSERT(false, "RendererAPI not implemented in buffer's switch");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) 
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		GR_CORE_ASSERT(false, "RendererAPI not implemented in buffer's switch");
		return nullptr;
	}

}