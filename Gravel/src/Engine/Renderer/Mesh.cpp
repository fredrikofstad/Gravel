#include "grpch.h"

#include "Renderer.h"
#include "Mesh.h"
#include "Platform/OpenGL/OpenGLMesh.h"

namespace Gravel {

	Shared<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: GR_CORE_ASSERT(false, "Renderer None not yet implemented"); return nullptr;
		case RendererAPI::API::OpenGL: return MakeShared<OpenGLMesh>(vertices, indices, textures);
		default: GR_CORE_ASSERT(false, "RendererAPI not implemented in Mesh's switch"); return nullptr;
		}
	}

}