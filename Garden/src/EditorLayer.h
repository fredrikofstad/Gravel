#pragma once

#include <Gravel.h>

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

		Shared<Texture2D> m_defaultTexture;
		Shared<Texture2D> m_texture;
		Shared<Texture2D> m_kappaTexture;
		Shared<SubTexture> m_bush;

		Shared<FrameBuffer> m_frameBuffer;

		glm::vec4 m_materialColor = { 0.3, 0.6, 0.34, 1.0f };

		OrthographicCameraConrtoller m_cameraController;

		uint32_t m_mapWidth, m_mapHeight;
		std::unordered_map<char, Shared<SubTexture>> s_textureMap;

	};

}