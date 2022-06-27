#pragma once

#include "OrthographicCamera.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Gravel {

	struct OrhographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return m_camera; }
		const OrthographicCamera& GetCamera() const { return m_camera; }

		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		float GetZoom() const { return m_zoom; }
		void SetZoom(float level) { m_zoom = level; CalculateView(); }

		const OrhographicCameraBounds& GetBounds() const { return m_bounds; }

	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_aspectRatio;
		float m_zoom = 1.0f;
		bool m_rotation;
		
		OrthographicCamera m_camera;
		OrhographicCameraBounds m_bounds;

		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraRotation = 0;
		float m_moveSpeed = 1.0f;
		float m_rotationSpeed = 180.0f;

	};


}