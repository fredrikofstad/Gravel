#include "EditorLayer.h"
#include "Imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Gravel {

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



	EditorLayer::EditorLayer()
		:Layer("Test 2D"), m_cameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		GR_PROFILE_FUNCTION();

		m_defaultTexture = Texture2D::Create("res/textures/default.png");
		m_texture = Texture2D::Create("res/textures/panda.png");
		m_kappaTexture = Texture2D::Create("res/textures/kappa.png");
		m_bush =  SubTexture::CreateFromCoords(m_kappaTexture, { 2, 12 }, { 16,16 });
		//framebuffer
		FrameBufferSpecification frameBufferSpecs;
		frameBufferSpecs.Width = 1280;
		frameBufferSpecs.Height = 720;
		m_frameBuffer = FrameBuffer::Create(frameBufferSpecs);

		//Texture atlas test

		m_mapWidth = s_mapWidth;
		m_mapHeight = strlen(s_map) / s_mapWidth;

		// water 3, 9
		s_textureMap['w'] = SubTexture::CreateFromCoords(m_kappaTexture, { 3, 9 }, { 16,16 });
		// dirt 2, 11
		s_textureMap['d'] = SubTexture::CreateFromCoords(m_kappaTexture, { 2, 11 }, { 16,16 });
		//grass 4, 14
		s_textureMap['g'] = SubTexture::CreateFromCoords(m_kappaTexture, { 4, 14 }, { 16,16 });


		m_cameraController.SetZoom(5.0f);

	}

	void EditorLayer::OnDetach()
	{
		GR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep deltaTime)
	{
		GR_PROFILE_FUNCTION();

		{
			GR_PROFILE_SCOPE("CameraController::OnUpdate");
			m_cameraController.OnUpdate(deltaTime);
		}

		Renderer2D::ResetStatistics();

		{
			GR_PROFILE_SCOPE("Renderer Prep");
			m_frameBuffer->Bind();
			RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderInstruction::Clear();
		}

		{
			GR_PROFILE_SCOPE("Renderer Draw");

#if 0
			static float rotation = 0.0f;
			rotation += deltaTime * 50.0f;

			Renderer2D::StartScene(m_cameraController.GetCamera());

			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.2f, 0.2f, 0.85f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.14f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, 1.0f, 0.0f }, { 1.6f, 1.6f }, m_materialColor);
			Renderer2D::DrawQuad({ 0.5f, 1.0f, 0.1f }, { 1.0f, 1.5f }, m_texture, 1.0f, { 1.0f, 1.0f, 0.5f, 0.5f });
			Renderer2D::DrawQuad({ -0.5f, 1.0f }, { 1.0f, 1.5f }, m_texture, 1.0f, { 1.0f,0.0f,1.0f,1.0f });
			Renderer2D::DrawRotateQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_texture, 10.0f);
			Renderer2D::DrawRotateQuad({ 2.0f, 0.0f, 0.1f }, { 1.0f, 1.5f }, -rotation, m_texture, 1.0f);

			Renderer2D::EndScene();
#endif
#if 1
			static float rotation = 0.0f;
			rotation += deltaTime * 50.0f;

			Renderer2D::StartScene(m_cameraController.GetCamera());
			for (uint32_t y = 0; y < m_mapHeight; y++)
			{
				for (uint32_t x = 0; x < m_mapWidth; x++)
				{
					char tileType = s_map[x + y * m_mapWidth];
					Shared<SubTexture> texture;
					if (s_textureMap.find(tileType) != s_textureMap.end())
						texture = s_textureMap[tileType];
					else
						texture = m_bush;
					Renderer2D::DrawQuad({ x - m_mapWidth / 2.0f , m_mapWidth - y - m_mapWidth / 2.0f, 0.0f }, { 1.0f, 1.0f }, texture);
				}
			}


			Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.1f }, { 0.8f, 0.8f }, { 0.2f, 0.2f, 0.85f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.1f }, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.14f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, 1.0f, 0.1f }, { 1.6f, 1.6f }, m_materialColor);
			Renderer2D::DrawQuad({ 0.5f, 1.0f, 0.2f }, { 1.0f, 1.5f }, m_texture, 1.0f, { 1.0f, 1.0f, 0.5f, 0.5f });
			Renderer2D::DrawQuad({ -0.5f, 1.0f, 0.1f }, { 1.0f, 1.5f }, m_texture, 1.0f, { 1.0f,0.0f,1.0f,1.0f });
			Renderer2D::DrawRotateQuad({ -2.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, rotation, m_texture, 10.0f);
			Renderer2D::DrawRotateQuad({ 2.0f, 0.0f, 0.2f }, { 1.0f, 1.5f }, -rotation, m_texture, 1.0f);

			Renderer2D::EndScene();
			m_frameBuffer->Unbind();

#endif
		}

	}

	void EditorLayer::OnImguiRender()
	{
		GR_PROFILE_FUNCTION();


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

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto statistics = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D statistics:");
		ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
		ImGui::Text("Quad Count: %d", statistics.QuadCount);
		ImGui::Text("Vertices: %d", statistics.GetTotalVertexCount());
		ImGui::Text("Indices: %d", statistics.GetTotalIndexCount());
		ImGui::End();

		ImGui::Begin("Material");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_materialColor));
		RendererID textureID = m_defaultTexture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();


		ImGui::Begin("Viewport");
		RendererID colorID = m_frameBuffer->GetColorAttachment();
		ImGui::Image((void*)colorID, ImVec2{ 1280.0f, 720.0f }, ImVec2{0, 1}, ImVec2{ 1, 0 }); //imvec to flip frame
		ImGui::End();

		ImGui::End();


	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_cameraController.OnEvent(e);
	}

}
