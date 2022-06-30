#include "grpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Gravel {

	SceneCamera::SceneCamera()
	{
		Recalculate();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_orthographicSize = size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;
		Recalculate();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_aspectRatio = (float)width / (float)height;
		Recalculate();
	}

	void SceneCamera::Recalculate()
	{
		float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
		float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
		float orthoBottom = -m_orthographicSize * 0.5f;
		float orthoTop = m_orthographicSize * 0.5f;

		m_projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
	}

}