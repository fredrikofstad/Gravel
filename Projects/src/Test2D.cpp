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
	m_texture = Gravel::Texture2D::Create("res/textures/panda.png");

}

void Test2D::OnDetach()
{

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

		Gravel::Renderer2D::StartScene(m_cameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		Gravel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.2f, 0.2f, 0.85f, 1.0f });
		Gravel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.14f, 1.0f });
		Gravel::Renderer2D::DrawQuad({ 0.2f, 0.5f }, { 0.5f, 0.75f }, m_texture);

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
