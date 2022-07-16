#include "EditorLayer.h"
#include "Imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Math/Math.h"

#include "Engine/Scene/SceneSerializer.h"

#include "Engine/Utilities/PlatformUtilities.h"
#include "imguizmo/ImGuizmo.h"


namespace Gravel {

	extern const std::filesystem::path g_resPath;


	EditorLayer::EditorLayer()
		:Layer("Garden")
	{

	}

	void EditorLayer::OnAttach()
	{
		GR_PROFILE_FUNCTION();

		m_iconTexture = Texture2D::Create("res/textures/icon.png");
		m_defaultTexture = Texture2D::Create("res/textures/default.png");

		m_playIcon = Texture2D::Create("res/ui/icons/icon-play.png");
		m_stopIcon = Texture2D::Create("res/ui/icons/icon-stop.png");



		FrameBufferSpecification frameBufferSpecs;
		frameBufferSpecs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

		frameBufferSpecs.Width = 1280;
		frameBufferSpecs.Height = 720;
		m_frameBuffer = FrameBuffer::Create(frameBufferSpecs);

		m_scene = MakeShared<Scene>();
		m_camera = GardenCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_hierarchyPanel.SetScene(m_scene);
	}

	void EditorLayer::OnDetach()
	{
		GR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep deltaTime)
	{
		GR_PROFILE_FUNCTION();

		if (FrameBufferSpecification specification = m_frameBuffer->GetSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && 
			(specification.Width != m_viewportSize.x || specification.Height != m_viewportSize.y))
		{
			m_frameBuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);

			m_camera.SetViewportSize(m_viewportSize.x, m_viewportSize.y);

			m_scene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		// Setup

		m_camera.OnUpdate(deltaTime);


		Renderer2D::ResetStatistics();

		m_frameBuffer->Bind();
		RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderInstruction::Clear();

		m_frameBuffer->ClearAttachment(1, -1);

		// modes

		switch (m_sceneState)
		{
			case SceneState::Editor:
			{
				m_scene->OnUpdateEditor(deltaTime, m_camera);
				break;
			}

			case SceneState::Player:
			{
				m_scene->OnUpdateRuntime(deltaTime);
				break;
			}

		}


		// scene

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_frameBuffer->ReadPixel(1, mouseX, mouseY);
			m_hoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_scene.get());

		}

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
		ImGuiStyle& style = ImGui::GetStyle();
		float minSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 340.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minSize;


		if (ImGui::BeginMenuBar())
		{
			RendererID iconID = m_iconTexture->GetRendererID();
			ImGui::Image((void*)iconID, ImVec2{ 20.0f, 20.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

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

		m_hierarchyPanel.OnImguiRender();
		m_browserPanel.OnImGuiRender();


		ImGui::Begin("Settings");

		std::string name = "None";
		if (m_hoveredEntity)
			name = m_hoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto statistics = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D statistics:");
		ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
		ImGui::Text("Quad Count: %d", statistics.QuadCount);
		ImGui::Text("Vertices: %d", statistics.GetTotalVertexCount());
		ImGui::Text("Indices: %d", statistics.GetTotalIndexCount());
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportFocused && !m_viewportHovered);

		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { availableSize.x, availableSize.y };

		RendererID colorID = m_frameBuffer->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(colorID), ImVec2{ (float)m_viewportSize.x, (float)m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }); //imvec to flip frame
		
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_resPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		//Gizmo

		Entity selectedEntity = m_hierarchyPanel.GetSelected();
		if (selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);

			// Runtime Camera
			/*
			auto cameraEntity = m_scene->GetPrimaryCamera();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
			*/

			// Editor camera
			const glm::mat4& cameraProjection = m_camera.GetProjection();
			glm::mat4 cameraView = m_camera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Position = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
		
		ImGui::PopStyleVar();
		ImGui::End();

		DisplayToolbar();

		ImGui::End();
	}


	void EditorLayer::DisplayToolbar()
	{
		//consider making global color scheme
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 2));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));


		ImGui::Begin("toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		
		float size = ImGui::GetWindowHeight() - 8.0f;
		Shared<Texture> icon = m_sceneState == SceneState::Editor ? m_playIcon : m_stopIcon;
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			switch (m_sceneState)
			{
				case SceneState::Editor:
					OnScenePlay();
					break;
				case SceneState::Player:
					OnSceneStop();
					break;
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_camera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GR_BIND_EVENT(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(GR_BIND_EVENT(EditorLayer::OnMouseButtonPressed));

	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();
				else if (control)
					SaveScene();

				//gizmo
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			// Gizmos
			case Key::A:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = -1;
				break;
			}
			case Key::M:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_hierarchyPanel.SetSelected(m_hoveredEntity);
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_scene = MakeShared<Scene>();
		m_scene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_hierarchyPanel.SetScene(m_scene);
	}

	void EditorLayer::OpenScene()
	{
		std::optional<std::string> filepath = FileDialogs::OpenFile("Garden Scene (*.zen)\0*.zen\0");
		if (filepath)
			OpenScene(filepath.value());
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_currentScenePath = path;
		m_scene = MakeShared<Scene>();
		m_scene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_hierarchyPanel.SetScene(m_scene);

		SceneSerializer serializer(m_scene);
		serializer.Deserialize(m_currentScenePath.string());
	}

	void EditorLayer::SaveScene()
	{
		SceneSerializer serializer(m_scene);
		serializer.Serialize(m_currentScenePath.string());
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filepath = FileDialogs::SaveFile("Garden Scene (*.zen)\0*.zen\0");
		if (filepath)
		{
			SceneSerializer serializer(m_scene);
			serializer.Serialize(*filepath);
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_sceneState = SceneState::Player;
	}

	void EditorLayer::OnSceneStop()
	{
		m_sceneState = SceneState::Editor;
	}


}
