#include "grpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Gravel {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented in VertexArray's switch"); return nullptr;
		}

	}

}										