#include "grpch.h"
#include "SubTexture.h"

namespace Gravel {




	SubTexture::SubTexture(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_texture(texture)
	{
		m_coordinates[0] = { min.x, min.y };
		m_coordinates[1] = { max.x, min.y };
		m_coordinates[2] = { max.x, max.y };
		m_coordinates[3] = { min.x, max.y };

	}

	Shared<SubTexture> SubTexture::CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2 coordinates, const glm::vec2 tileSize, const glm::vec2 ratio)
	{
		glm::vec2 min = { (coordinates.x * tileSize.x) / texture->GetWidth(), (coordinates.y * tileSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coordinates.x + ratio.x) * tileSize.x) / texture->GetWidth(), ((coordinates.y + ratio.y) * tileSize.y) / texture->GetHeight() };
		return MakeShared<SubTexture>(texture, min, max);
	}

}