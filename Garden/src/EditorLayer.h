#pragma once

#include <Gravel.h>
#include "Panels/HierarchyPanel.h"

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

		Shared<Texture2D> m_iconTexture;
		Shared<Texture2D> m_defaultTexture;

		OrthographicCameraController m_cameraController;
		Shared<Scene> m_scene;
		Entity m_cameraEntity;
		Entity m_secondCamera;

		bool m_primaryCamera = true;

		Entity m_panda;

		Shared<FrameBuffer> m_frameBuffer;

		bool m_viewportFocused = false, m_viewportHovered = false;
		glm::vec2 m_viewportSize = {0,0};

		// panels

		HierarchyPanel m_hierarchyPanel;

	};

}