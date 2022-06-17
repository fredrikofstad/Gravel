#include "grpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Gravel {


	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
	{
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::RecalculateMatrix()
	{
		// multiplying with identity matrix to construct the 4x4
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
		// inversing so it's ready to multiply with models
		m_viewMatrix = glm::inverse(transform);
		// for openGL projection * view
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

}