#include "EditorLayer.h"
#include "Imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Gravel {


	/*
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

	*/



	EditorLayer::EditorLayer()
		:Layer("Test 2D"), m_cameraController(1280.0f / 720.0f, false)
	{
	}

	void EditorLayer::OnAttach()
	{
		GR_PROFILE_FUNCTION();

		m_iconTexture = Texture2D::Create("res/textures/icon.png");

		m_defaultTexture = Texture2D::Create("res/textures/default.png");
		//m_kappaTexture = Texture2D::Create("res/textures/kappa.png");
		//m_bush =  SubTexture::CreateFromCoords(m_kappaTexture, { 2, 12 }, { 16,16 });
		//framebuffer
		FrameBufferSpecification frameBufferSpecs;
		frameBufferSpecs.Width = 1280;
		frameBufferSpecs.Height = 720;
		m_frameBuffer = FrameBuffer::Create(frameBufferSpecs);

		m_scene = MakeShared<Scene>();
		m_panda = m_scene->CreateEntity("Panda");
		m_panda.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.8f, 0.0f, 1.0f });

		/*

		//Texture atlas test

		m_mapWidth = s_mapWidth;
		m_mapHeight = strlen(s_map) / s_mapWidth;

		// water 3, 9
		s_textureMap['w'] = SubTexture::CreateFromCoords(m_kappaTexture, { 3, 9 }, { 16,16 });
		// dirt 2, 11
		s_textureMap['d'] = SubTexture::CreateFromCoords(m_kappaTexture, { 2, 11 }, { 16,16 });
		//grass 4, 14
		s_textureMap['g'] = SubTexture::CreateFromCoords(m_kappaTexture, { 4, 14 }, { 16,16 });


		*/

		m_cameraController.SetZoom(5.0f);

	}

	void EditorLayer::OnDetach()
	{
		GR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep deltaTime)
	{
		GR_PROFILE_FUNCTION();

		// Setup
		if (m_viewportFocused)
			m_cameraController.OnUpdate(deltaTime);

		Renderer2D::ResetStatistics();

		m_frameBuffer->Bind();
		RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderInstruction::Clear();

		Renderer2D::StartScene(m_cameraController.GetCamera());

		/*

		// manipulating values
		m_timeToNextFrame -= deltaTime;
		if (m_timeToNextFrame < 0)
		{
			m_timeToNextFrame = m_timeToWait;
			s_textureMap['w']->NextFrame(4);
		}

		//Rendering


		// tilemap
		
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
				Renderer2D::DrawQuad({ x - m_mapWidth / 2.0f, m_mapWidth - y - m_mapWidth / 2.0f - 6.0f, 0.0f }, { 1.0f, 1.0f }, texture);
			}
		}
		*/

		// scene
		m_scene->OnUpdate(deltaTime);

		Renderer2D::EndScene();
		m_frameBuffer->Unbind();

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
		ImGui::Begin("Gravel", &dockspaceOpen, window_flags);
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
			RendererID iconID = m_iconTexture->GetRendererID();
			ImGui::Image((void*)iconID, ImVec2{ 20.0f, 20.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportFocused || !m_viewportHovered);

		ImVec2 availableSize = ImGui::GetContentRegionAvail();

		if (m_viewportSize != *((glm::vec2*)&availableSize))
		{
			m_frameBuffer->Resize((uint32_t)availableSize.x, (uint32_t)availableSize.y);
			m_viewportSize = { availableSize.x, availableSize.y };

			m_cameraController.OnResize(availableSize.x, availableSize.y);
		}
		RendererID colorID = m_frameBuffer->GetColorAttachment();
		ImGui::Image((void*)colorID, ImVec2{ (float)m_viewportSize.x, (float)m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }); //imvec to flip frame
		ImGui::PopStyleVar();
		ImGui::End();



		ImGui::Begin("Settings");

		auto statistics = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D statistics:");
		ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
		ImGui::Text("Quad Count: %d", statistics.QuadCount);
		ImGui::Text("Vertices: %d", statistics.GetTotalVertexCount());
		ImGui::Text("Indices: %d", statistics.GetTotalIndexCount());
		ImGui::End();

		ImGui::Begin("Material");

		if (m_panda)
		{
			ImGui::Separator();
			auto& tag = m_panda.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());

			auto& pandaColor = m_panda.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Color", glm::value_ptr(pandaColor));
			ImGui::Separator();
		}
		ImGui::DragFloat("Animation Rate", &m_timeToWait, 0.005f, 0.0f, 2.0f);
		ImGui::End();


		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_cameraController.OnEvent(e);
	}

}
