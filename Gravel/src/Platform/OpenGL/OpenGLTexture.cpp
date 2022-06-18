#include "grpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>


namespace Gravel {


	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: m_filepath(filepath)
	{
		int width, height, channels;
		// using stb_image to import asset, provide references
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		GR_CORE_ASSERT(data, "Failed to load texture image.");
		m_width = width;
		m_height = height;

		// take image buffer and upload to OpenGL

		// create textures in gl format
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		// allocate memory for texture storage
		glTextureStorage2D(m_rendererID, 1, GL_RGBA8, m_width, m_height);
		// set how we shrink or expand texture if not 1 to 1 mapping
		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// upload texture
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// free memory from data pointer
		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}

}