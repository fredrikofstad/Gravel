#pragma once

#include <glm/glm.hpp>
#include "Texture.h"
#include "Engine/Core/Timestep.h"

namespace Gravel {

	class SubTexture
	{
	public:
		SubTexture(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		SubTexture(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2 tileCoordinates, const glm::vec2 tileSize);

		const Shared<Texture2D> GetTexture() const { return m_texture; }
		const glm::vec2* GetCoordinates() const { return m_coordinates; }

		void NextFrame(uint32_t frames);

		static Shared<SubTexture> CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2 coordinates, const glm::vec2 tileSize, const glm::vec2 ratio = { 1, 1 });

	private:
		Shared<Texture2D> m_texture;

		glm::vec2 m_coordinates[4];
		glm::vec2 m_tileCoordinates;
		glm::vec2 m_tileSize;
		uint32_t m_currentFrame = 0;
	};

}