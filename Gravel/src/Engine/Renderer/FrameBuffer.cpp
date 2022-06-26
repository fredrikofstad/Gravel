#include "grpch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Gravel {

	Shared<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer::None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return MakeShared<OpenGLFrameBuffer>(specification);
			default: GR_CORE_ASSERT(false, "RendererAPI not implemented."); return nullptr;
		}
	}


}