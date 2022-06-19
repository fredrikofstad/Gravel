#include "grpch.h"
#include "RenderInstruction.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Gravel {


	// in future specify platform dynamically
	// OpenGL for now

	Unique<RendererAPI> RenderInstruction::s_rendererAPI = MakeUnique<OpenGLRendererAPI>();

}
