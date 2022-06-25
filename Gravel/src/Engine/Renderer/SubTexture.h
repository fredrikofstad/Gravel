#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

namespace Gravel {

	class SubTexture
	{
	public:
		SubTexture(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		const Shared<Texture2D> GetTexture() const { return m_texture; }
		const glm::vec2* GetCoordinates() const { return m_coordinates; }

		static Shared<SubTexture> CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2 coordinates, const glm::vec2 tileSize, const glm::vec2 ratio = {1, 1});
	private:
		Shared<Texture2D> m_texture;
		glm::vec2 m_coordinates[4];
	};


}