#pragma once

#include "Engine/Renderer/FrameBuffer.h"

namespace Gravel {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer() override;

		void Recalculate();

		virtual void Bind() override;
		virtual void Unbind() override;
		void Resize(uint32_t width, uint32_t height) override;


		//virtual const FrameBufferSpecification& GetSpecification() { return m_specification; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_specification; }
		virtual uint32_t GetColorAttachment() const override { return m_colorAttachment; }


	private:
		FrameBufferSpecification m_specification;
		uint32_t m_colorAttachment = 0, m_depthAttachment = 0;
		RendererID m_rendererID = 0;
	};

}