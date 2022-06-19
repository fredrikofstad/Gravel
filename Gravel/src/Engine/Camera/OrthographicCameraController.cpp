#include "grpch.h"
#include "OrthographicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

namespace Gravel {

	OrthographicCameraConrtoller::OrthographicCameraConrtoller(float aspectRatio, bool rotation)
		: m_camera(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom), 
		  m_aspectRatio(aspectRatio), m_rotation(rotation)
	{

	}

	void OrthographicCameraConrtoller::OnUpdate(Timestep deltaTime)
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

	void OrthographicCameraConrtoller::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GR_BIND_EVENT(OrthographicCameraConrtoller::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GR_BIND_EVENT(OrthographicCameraConrtoller::OnWindowResized));
	}

	bool OrthographicCameraConrtoller::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoom -= e.GetYOffset()* 0.25f;
		m_zoom = std::max(m_zoom, 0.25f);
		m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
		return false;
	}

	bool OrthographicCameraConrtoller::OnWindowResized(WindowResizeEvent& e)
	{
		m_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
		return false;
	}

}