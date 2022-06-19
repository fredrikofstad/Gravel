#pragma once

#include "OrthographicCamera.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Gravel {

	class OrthographicCameraConrtoller
	{
	public:
		OrthographicCameraConrtoller(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return m_camera; }
		const OrthographicCamera& GetCamera() const { return m_camera; }

		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& e);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_aspectRatio;
		float m_zoom = 1.0f;
		bool m_rotation;
		
		OrthographicCamera m_camera;

		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraRotation = 0;
		float m_moveSpeed = 1.0f;
		float m_rotationSpeed = 180.0f;



	};


}