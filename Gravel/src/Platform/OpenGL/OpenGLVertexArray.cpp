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
			switch (attribute.Type)
			{
			case AttributeType::Float:
			case AttributeType::Float2:
			case AttributeType::Float3:
			case AttributeType::Float4:
			{
				glEnableVertexAttribArray(m_vertexBufferIndex);
				glVertexAttribPointer(m_vertexBufferIndex,
					attribute.GetCount(),
					AttributeToGLType(attribute.Type),
					attribute.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)attribute.Offset);
				m_vertexBufferIndex++;
				break;
			}
			case AttributeType::Int:
			case AttributeType::Int2:
			case AttributeType::Int3:
			case AttributeType::Int4:
			case AttributeType::Bool:
			{
				glEnableVertexAttribArray(m_vertexBufferIndex);
				glVertexAttribIPointer(m_vertexBufferIndex,
					attribute.GetCount(),
					AttributeToGLType(attribute.Type),
					layout.GetStride(),
					(const void*)attribute.Offset);
				m_vertexBufferIndex++;
				break;
			}
			case AttributeType::Mat3:
			case AttributeType::Mat4:
			{
				uint8_t count = attribute.GetCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_vertexBufferIndex);
					glVertexAttribPointer(m_vertexBufferIndex,
						count,
						AttributeToGLType(attribute.Type),
						attribute.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(attribute.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_vertexBufferIndex, 1);
					m_vertexBufferIndex++;
				}
				break;
			}
			default:
				GR_CORE_ASSERT(false, "Unknown type");
			}
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