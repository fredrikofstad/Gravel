#include "grpch.h"
#include "OrthographicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

namespace Gravel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_camera(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom), 
		  m_aspectRatio(aspectRatio), m_rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
	{
		if (Input::isKeyPressed(GR_KEY_A))
			m_cameraPosition.x -= m_moveSpeed * deltaTime;

		else if (Input::isKeyPressed(GR_KEY_D))
			m_cameraPosition.x += m_moveSpeed * deltaTime;

		if (Input::isKeyPressed(GR_KEY_W))
			m_cameraPosition.y += m_moveSpeed * deltaTime;

		else if (Input::isKeyPressed(GR_KEY_S))
			m_cameraPosition.y -= m_moveSpeed * deltaTime;

		if(m_rotation)
		{
			if (Input::isKeyPressed(GR_KEY_Q))
				m_cameraRotation += m_rotationSpeed * deltaTime;

			if (Input::isKeyPressed(GR_KEY_E))
				m_cameraRotation -= m_rotationSpeed * deltaTime;
		
			m_camera.SetRotation(m_cameraRotation);
		}

		m_camera.SetPosition(m_cameraPosition);

		m_moveSpeed = m_zoom;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GR_BIND_EVENT(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GR_BIND_EVENT(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateView()
	{
		m_bounds = { -m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom };
		m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_aspectRatio = width / height;
		m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoom -= e.GetYOffset()* 0.25f;
		m_zoom = std::max(m_zoom, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}