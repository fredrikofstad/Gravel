#pragma once

#include <glm/glm.hpp>


namespace Gravel {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec3& position) 
		{
			m_position = position; 
			RecalculateMatrix(); 
		}
		const float GetRotation() const{ return m_rotation; }
		void SetRotation(const float rotation) { m_rotation = rotation; }


		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& GetViewnMatrix() const { return m_viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

	private:
		void RecalculateMatrix();
	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;
	};

}