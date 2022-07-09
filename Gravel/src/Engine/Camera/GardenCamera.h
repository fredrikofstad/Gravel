#pragma once

#include "Camera.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Gravel {

	class GardenCamera : public Camera
	{
	public:
		GardenCamera() = default;
		GardenCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_distance; }
		inline void SetDistance(float distance) { m_distance = distance; }

		inline void SetViewportSize(float width, float height) { m_viewportWidth = width; m_viewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		glm::mat4 GetViewProjection() const { return m_projection * m_viewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_aspectRatio = 1.778f, m_nearClip = 0.1f, m_farClip = 1000.0f;

		glm::mat4 m_viewMatrix;
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_focalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_initialMousePosition = { 0.0f, 0.0f };

		float m_distance = 10.0f;
		float m_pitch = 0.0f, m_Yaw = 0.0f;

		float m_viewportWidth = 1280, m_viewportHeight = 720;
	};

}