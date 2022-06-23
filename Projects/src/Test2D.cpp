#include "Test2D.h"
#include "Imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


Test2D::Test2D()
	:Layer("Test 2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Test2D::OnAttach()
{
	GR_PROFILE_FUNCTION();

	m_texture = Gravel::Texture2D::Create("res/textures/panda.png");
}

void Test2D::OnDetach()
{
	GR_PROFILE_FUNCTION();
}

void Test2D::OnUpdate(Gravel::Timestep deltaTime)
{
	GR_PROFILE_FUNCTION();

	{
		GR_PROFILE_SCOPE("CameraController::OnUpdate");
		m_cameraController.OnUpdate(deltaTime);
	}

	{
		GR_PROFILE_SCOPE("Renderer Prep");
		Gravel::RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gravel::RenderInstruction::Clear();
	}

	{
		GR_PROFILE_SCOPE("Renderer Draw");

		static float rotation = 0.0f;
		rotation += deltaTime * 50.0f;

		Gravel::Renderer2D::StartScene(m_cameraController.GetCamera());

		Gravel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.2f, 0.2f, 0.85f, 1.0f });
		Gravel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.14f, 1.0f });
		Gravel::Renderer2D::DrawQuad({ 0.5f, 1.0f, 0.0f }, { 1.6f, 1.6f }, m_materialColor);
		Gravel::Renderer2D::DrawQuad({ 0.5f, 1.0f, 0.1f }, { 1.0f, 1.5f }, m_texture, 1.0f, {1.0f, 1.0f, 0.5f, 0.5f});
		Gravel::Renderer2D::DrawQuad({ -0.5f, 1.0f }, { 1.0f, 1.5f }, m_texture, 1.0f, {1.0f,0.0f,1.0f,1.0f});
		Gravel::Renderer2D::DrawRotateQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_texture, 10.0f);
		Gravel::Renderer2D::DrawRotateQuad({  2.0f, 0.0f, 0.1f }, { 1.0f, 1.5f }, -rotation, m_texture, 1.0f);



		Gravel::Renderer2D::EndScene();
	}

}

void Test2D::OnImguiRender()
{
	GR_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_materialColor));

	ImGui::End();
}

void Test2D::OnEvent(Gravel::Event& e)
{
	m_cameraController.OnEvent(e);
}
