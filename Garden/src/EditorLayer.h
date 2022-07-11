#pragma once

#include <Gravel.h>
#include "Panels/HierarchyPanel.h"
#include "Panels/BrowserPanel.h"
#include "Engine/Camera/GardenCamera.h"

namespace Gravel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Timestep deltaTime) override;
		virtual void OnImguiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:

		Shared<Texture2D> m_iconTexture;
		Shared<Texture2D> m_defaultTexture;

		Shared<Scene> m_scene;

		bool m_primaryCamera = true;
		GardenCamera m_camera;
		Entity m_hoveredEntity;

		Shared<FrameBuffer> m_frameBuffer;

		bool m_viewportFocused = false, m_viewportHovered = false;
		glm::vec2 m_viewportSize = {0,0};
		glm::vec2 m_viewportBounds[2];


		int m_gizmoType = -1;

		// panels
		HierarchyPanel m_hierarchyPanel;
		BrowserPanel m_browserPanel;

	};

}