#pragma once

#include "Engine/Renderer/FrameBuffer.h"

namespace Gravel {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer();

		void Resize();

		virtual void Bind() override;
		virtual void Unbind() override;

		//virtual const FrameBufferSpecification& GetSpecification() { return m_specification; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_specification; }
		virtual uint32_t GetColorAttachment() const override { return m_colorAttachment; }


	private:
		FrameBufferSpecification m_specification;
		uint32_t m_colorAttachment, m_depthAttachment;
		RendererID m_rendererID;
	};

}