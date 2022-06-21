
#pragma once

#include <Gravel.h>

class ExampleLayer : public Gravel::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gravel::Timestep deltaTime) override;
	virtual void OnImguiRender() override;
	void OnEvent(Gravel::Event& event) override;
private:
	Gravel::ShaderLibrary m_shaderLibrary;
	Gravel::Shared<Gravel::VertexArray> m_vertexArray;
	Gravel::Shared<Gravel::Shader> m_shader;

	Gravel::Shared<Gravel::VertexArray> m_squareVAO;
	Gravel::Shared<Gravel::Shader> m_flatColorShader;

	Gravel::Shared<Gravel::Texture2D> m_kanariTexture;

	Gravel::OrthographicCameraConrtoller m_cameraController;

	glm::vec3 m_squareColor = { 0.3, 0.6, 0.34 };
};
