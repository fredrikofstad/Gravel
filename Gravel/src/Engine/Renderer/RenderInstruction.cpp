#include "grpch.h"
#include "RenderInstruction.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Gravel {


	// in future specify platform dynamically
	// OpenGL for now

	RendererAPI* RenderInstruction::s_rendererAPI = new OpenGLRendererAPI;

}
