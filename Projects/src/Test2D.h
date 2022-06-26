#pragma once

#include <Gravel.h>

class Test2D : public Gravel::Layer
{
public:
	Test2D();
	virtual ~Test2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Gravel::Timestep deltaTime) override;
	virtual void OnImguiRender() override;
	virtual void OnEvent(Gravel::Event& e) override;
private:

	Gravel::Shared<Gravel::Texture2D> m_defaultTexture;
	Gravel::Shared<Gravel::Texture2D> m_texture;
	Gravel::Shared<Gravel::Texture2D> m_kappaTexture;
	Gravel::Shared<Gravel::SubTexture> m_bush;

	Gravel::Shared<Gravel::FrameBuffer> m_frameBuffer;

	glm::vec4 m_materialColor = { 0.3, 0.6, 0.34, 1.0f };

	Gravel::OrthographicCameraConrtoller m_cameraController;

	uint32_t m_mapWidth, m_mapHeight;
	std::unordered_map<char, Gravel::Shared<Gravel::SubTexture>> s_textureMap;

};