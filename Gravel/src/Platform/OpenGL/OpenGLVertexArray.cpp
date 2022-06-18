#include "grpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Gravel {

	static GLenum AttributeToGLType(AttributeType type)
	{
		switch (type)
		{
			case Gravel::AttributeType::Int:     return GL_INT;
			case Gravel::AttributeType::Int2:    return GL_INT;
			case Gravel::AttributeType::Int3:    return GL_INT;
			case Gravel::AttributeType::Int4:    return GL_INT;
			case Gravel::AttributeType::Float:   return GL_FLOAT;
			case Gravel::AttributeType::Float2:  return GL_FLOAT;
			case Gravel::AttributeType::Float3:  return GL_FLOAT;
			case Gravel::AttributeType::Float4:  return GL_FLOAT;
			case Gravel::AttributeType::Mat3:    return GL_FLOAT;
			case Gravel::AttributeType::Mat4:    return GL_FLOAT;
			case Gravel::AttributeType::Bool:    return GL_BOOL;
			default: GR_CORE_ASSERT(false, "Unknown Attribute type."); return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_rendererID);

	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererID);

	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_rendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)
	{
		GR_CORE_ASSERT(vertexBuffer->GetLayout().GetAttributes().size(), "Vertex buffer has no layout.");
		
		glBindVertexArray(m_rendererID);
		vertexBuffer->Bind();

		uint32_t attributeIndex = 0;
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& attribute : layout)
		{
			glEnableVertexAttribArray(attributeIndex);
			glVertexAttribPointer(
				attributeIndex,
				attribute.GetCount(),
				AttributeToGLType(attribute.Type),
				attribute.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)attribute.Offset
			);
			attributeIndex++;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_rendererID);
		indexBuffer->Bind();

		m_indexBuffer = indexBuffer;
	}

}