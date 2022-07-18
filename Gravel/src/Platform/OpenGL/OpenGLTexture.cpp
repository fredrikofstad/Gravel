#include "grpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"


namespace Gravel {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_width(width), m_height(height), m_glFormat(GL_RGBA8), m_dataFormat(GL_RGBA)
	{
		// create textures in gl format
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		// allocate memory for texture storage
		glTextureStorage2D(m_rendererID, 1, m_glFormat, m_width, m_height);
		// set how we shrink or expand texture if not 1 to 1 mapping
		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: m_filepath(filepath), m_glFormat(GL_RGBA8), m_dataFormat(GL_RGBA)
	{
		int width, height, channels;
		// using stb_image to import asset, provide references
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		GR_CORE_ASSERT(data, "Failed to load texture image.");
		m_width = width;
		m_height = height;

		switch (channels)
		{
			case 3: m_glFormat = GL_RGB8; m_dataFormat = GL_RGB; break;
			case 4: m_glFormat = GL_RGBA8; m_dataFormat = GL_RGBA; break;
			default: GR_CORE_ASSERT(false, "Channel not supported.");
		}

		// take image buffer and upload to OpenGL

		// create textures in gl format
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		// allocate memory for texture storage
		glTextureStorage2D(m_rendererID, 1, m_glFormat, m_width, m_height);
		// set how we shrink or expand texture if not 1 to 1 mapping
		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// upload texture
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
		// free memory from data pointer
		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		GR_CORE_ASSERT(size == m_width * m_height * bpp, "Data must be the entire texture.");
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}

	GLint OpenGLTexture2D::LoadTextureFromFile(const char* path, const std::string& directory)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

}