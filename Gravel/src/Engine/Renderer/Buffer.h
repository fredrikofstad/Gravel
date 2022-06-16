#pragma once

#include <stdint.h>

namespace Gravel {

	enum class AttributeType
	{
		None, 
		Int, Int2, Int3, Int4, 
		Float, Float2, Float3, Float4,
		Mat3, Mat4, Bool
	};

	static uint32_t AttributeSize(AttributeType type) 
	{
		switch (type)
		{
			case AttributeType::Int:    return 4;
			case AttributeType::Int2:   return 4 * 2;
			case AttributeType::Int3:   return 4 * 3;
			case AttributeType::Int4:   return 4 * 4;
			case AttributeType::Float:  return 4;
			case AttributeType::Float2: return 4 * 2;
			case AttributeType::Float3: return 4 * 3;
			case AttributeType::Float4: return 4 * 4;
			case AttributeType::Mat3:   return 4 * 3 * 3;
			case AttributeType::Mat4:   return 4 * 4 * 4;
			case AttributeType::Bool:   return 1;
			default: GR_CORE_ASSERT(false, "Unknown Attribute type."); return 0;
		}
	}

	struct BufferAttribute
	{
		AttributeType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferAttribute() = default;

		BufferAttribute(AttributeType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(AttributeSize(type)), Offset(0), Normalized(normalized)
		{}

		uint32_t GetCount() const
		{
			switch (Type)
			{
				case AttributeType::Int:    return 1;
				case AttributeType::Int2:   return 2;
				case AttributeType::Int3:   return 3;
				case AttributeType::Int4:   return 4;
				case AttributeType::Float:  return 1;
				case AttributeType::Float2: return 2;
				case AttributeType::Float3: return 3;
				case AttributeType::Float4: return 4;
				case AttributeType::Mat3:   return 3 * 3;
				case AttributeType::Mat4:   return 4 * 4;
				case AttributeType::Bool:   return 1;
				default: GR_CORE_ASSERT(false, "Unknown Attribute type."); return 0;
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferAttribute>& attributes)
		: m_attributes(attributes) 
		{
			Calculate();
		}

		inline uint32_t GetStride() const { return m_stride; }
		inline const std::vector<BufferAttribute>& GetAttributes() const { return m_attributes; }
		
		std::vector<BufferAttribute>::iterator begin() { return m_attributes.begin(); }
		std::vector<BufferAttribute>::iterator end() { return m_attributes.end(); }
		std::vector<BufferAttribute>::const_iterator begin() const { return m_attributes.begin(); }
		std::vector<BufferAttribute>::const_iterator end() const { return m_attributes.end(); }

	private:
		void Calculate()
		{
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& attribute : m_attributes)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_stride += attribute.Size;
			}
		}
	private:
		std::vector<BufferAttribute> m_attributes;
		uint32_t m_stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);

	};

}