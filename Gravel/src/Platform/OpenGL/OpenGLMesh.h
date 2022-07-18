#pragma once

#include "Engine/Renderer/Mesh.h"
#include "glad/glad.h"

namespace Gravel {

	class OpenGLMesh : public Mesh
	{
	public:
		OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures);

		virtual ~OpenGLMesh() override = default;
		virtual void Draw(Shared<Shader> shader) override;

	private:
		void Init();

	private:
		GLuint VAO, VBO, EBO;

		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indices;
		std::vector<Texture> m_textures;
	};


}
