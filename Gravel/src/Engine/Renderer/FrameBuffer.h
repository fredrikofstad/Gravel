#pragma once

#include "Engine/Core/Core.h"

namespace Gravel {

	struct FrameBufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetColorAttachment() const = 0;

		//virtual const FrameBufferSpecification& GetSpecification() = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Shared<FrameBuffer> Create(const FrameBufferSpecification& specification);
	};


}