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
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		//virtual const FrameBufferSpecification& GetSpecification() { return m_specification; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_specification; }
		virtual uint32_t GetColorAttachment(uint32_t index = 0) const override { GR_CORE_ASSERT(index < m_colorAttachments.size(), "index over size"); return m_colorAttachments[index]; }


	private:
		FrameBufferSpecification m_specification;
		std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
		FramebufferTextureSpecification m_depthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment = 0;

		RendererID m_rendererID = 0;
	};

}