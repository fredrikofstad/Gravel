#pragma once

#include "Engine/Renderer/Texture.h"

namespace Gravel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_filepath;
		uint32_t m_width, m_height;
		RendererID m_rendererID;
	};

}