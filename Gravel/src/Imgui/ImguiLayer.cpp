#include "grpch.h"
#include "ImguiLayer.h"

#include "Imgui.h"
#include "Platform/OpenGL/ImguiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "Engine/Application.h"



namespace Gravel {
	Gravel::ImguiLayer::ImguiLayer()
		: Layer("ImGuiLayer")
	{

	}

	Gravel::ImguiLayer::~ImguiLayer()
	{

	}

	ImguiLayer::ImguiLayer()
	{

	}

	ImguiLayer::~ImguiLayer()
	{

	}

	void ImguiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TODO: should use key codes
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImguiLayer::OnDetach()
	{

	}

	void ImguiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImguiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(GR_BIND_EVENT(ImguiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(GR_BIND_EVENT(ImguiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(GR_BIND_EVENT(ImguiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(GR_BIND_EVENT(ImguiLayer::OnMouseScrolledEvent));

		//dispatcher.Dispatch<KeyTypedEvent>(GR_BIND_EVENT(ImguiLayer::OnKeyTypedEvent));
		//dispatcher.Dispatch<KeyPressedEvent>(GR_BIND_EVENT(ImguiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(GR_BIND_EVENT(ImguiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(GR_BIND_EVENT(ImguiLayer::OnWindowResizeEvent));

	}


	bool ImguiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;

		return false;
	}

	bool ImguiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{

	}

	bool ImguiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{

	}

	bool ImguiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{

	}

	bool ImguiLayer::OnKeyPressedEvent(MouseScrolledEvent& event)
	{

	}

	bool ImguiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{

	}

	bool ImguiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{

	}

	bool ImguiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{

	}

}