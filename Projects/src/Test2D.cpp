#include "Test2D.h"
#include "Imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const uint32_t s_mapWidth = 24;
static const char* s_map = 
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwdddddddddddddwwwww"
"wwwwdddgggggggggdddddwww"
"wwwwdgggggggggggggggddww"
"wwwdggggggggggggggggddww"
"wwwdddggggggggggggggddww"
"wwddddgggggggggggggggdww"
"wwwdddddgggggggggdddddww"
"wwwwwdddddddddddddddwwww"
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwccwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww"
;



Test2D::Test2D()
	:Layer("Test 2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Test2D::OnAttach()
{
	GR_PROFILE_FUNCTION();

	m_texture = Gravel::Texture2D::Create("res/textures/panda.png");
	m_kappaTexture = Gravel::Texture2D::Create("res/textures/kappa.png");
	m_bush = Gravel::SubTexture::CreateFromCoords(m_kappaTexture, { 2, 12 }, { 16,16 });
	m_tile = Gravel::SubTexture::CreateFromCoords(m_kappaTexture, { 0, 11 }, { 16,16 });
	m_tree = Gravel::SubTexture::CreateFromCoords(m_kappaTexture, { 0, 14 }, { 16,16 }, {2,2});

	//Texture atlas test

	m_mapWidth = s_mapWidth;
	m_mapHeight = strlen(s_map) / s_mapWidth;

	// water 3, 9
	s_textureMap['w'] = Gravel::SubTexture::CreateFromCoords(m_kappaTexture, {3, 9}, {16,16});
	// dirt 2, 11
	s_textureMap['d'] = Gravel::SubTexture::CreateFromCoords(m_kappaTexture, { 2, 11 }, { 16,16 });
	//grass 4, 14
	s_textureMap['g'] = Gravel::SubTexture::CreateFromCoords(m_kappaTexture, { 4, 14 }, { 16,16 });


	m_cameraController.SetZoom(5.0f);

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

	Gravel::Renderer2D::ResetStatistics();

	{
		GR_PROFILE_SCOPE("Renderer Prep");
		Gravel::RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gravel::RenderInstruction::Clear();
	}

	{
		GR_PROFILE_SCOPE("Renderer Draw");

#if 0
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
#endif
#if 1
		Gravel::Renderer2D::StartScene(m_cameraController.GetCamera());
		/*
		Gravel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_bush);
		Gravel::Renderer2D::DrawQuad({ 1.1f, 0.0f }, { 1.0f, 1.0f }, m_tile);
		Gravel::Renderer2D::DrawQuad({ 0.0f, 2.0f }, { 2.0f, 2.0f }, m_tree);
		*/
		//Gravel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_kappaTexture);
		
		for (uint32_t y = 0; y < m_mapHeight; y++)
		{
			for (uint32_t x = 0; x < m_mapWidth; x++)
			{
				char tileType = s_map[x + y * m_mapWidth];
				Gravel::Shared<Gravel::SubTexture> texture;
				if (s_textureMap.find(tileType) != s_textureMap.end())
					texture = s_textureMap[tileType];
				else
					texture = m_bush;
				Gravel::Renderer2D::DrawQuad({ x - m_mapWidth / 2.0f , m_mapWidth - y - m_mapWidth / 2.0f, 0.0f }, { 1.0f, 1.0f }, texture);
			}
		}

		Gravel::Renderer2D::EndScene();

#endif
	}

}

void Test2D::OnImguiRender()
{
	GR_PROFILE_FUNCTION();

	// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = false;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Gravel::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto statistics = Gravel::Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D statistics:");
		ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
		ImGui::Text("Quad Count: %d", statistics.QuadCount);
		ImGui::Text("Vertices: %d", statistics.GetTotalVertexCount());
		ImGui::Text("Indices: %d", statistics.GetTotalIndexCount());
		ImGui::End();

		ImGui::Begin("Material");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_materialColor));
		Gravel::RendererID textureID = m_texture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();
		}
		else
		{
			ImGui::Begin("Settings");

			auto statistics = Gravel::Renderer2D::GetStatistics();
			ImGui::Text("Renderer2D statistics:");
			ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
			ImGui::Text("Quad Count: %d", statistics.QuadCount);
			ImGui::Text("Vertices: %d", statistics.GetTotalVertexCount());
			ImGui::Text("Indices: %d", statistics.GetTotalIndexCount());
			ImGui::End();

			ImGui::Begin("Material");
			ImGui::ColorEdit4("Color", glm::value_ptr(m_materialColor));
			Gravel::RendererID textureID = m_texture->GetRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
			ImGui::End();
		}
}

void Test2D::OnEvent(Gravel::Event& e)
{
	m_cameraController.OnEvent(e);
}
