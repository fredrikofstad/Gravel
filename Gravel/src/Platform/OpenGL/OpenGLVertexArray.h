#pragma once

#include "Engine/Renderer/VertexArray.h"

namespace Gravel {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; }
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; }

	private:
		uint32_t m_rendererID;
		uint32_t m_vertexBufferIndex = 0;
		std::vector<Shared<VertexBuffer>> m_vertexBuffers;
		Shared<IndexBuffer> m_indexBuffer;
	};

}
