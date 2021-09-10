#include "clovepch.h"
#include "Clove/Core.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiLayer.h"

namespace Clove {

	ImGuiLayer::ImGuiLayer()
		: Layer()
	{

	}
	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::OnAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		/* Temporary until key mappings are implemented */
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
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");

	}
	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate() {

		ImGuiIO& io = ImGui::GetIO();
		GameApp& app = GameApp::GetInstance();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		float time = static_cast<float>(glfwGetTime());
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}
	void ImGuiLayer::OnEvent(Event& e) {
		ImGuiIO& io = ImGui::GetIO();
		EventDispatcher dp(e);

		dp.Dispatch<MouseButtonPressedEvent>(BIND_METHOD_1(ImGuiLayer::OnMouseButtonPressed));
		dp.Dispatch<MouseButtonReleasedEvent>(BIND_METHOD_1(ImGuiLayer::OnMouseButtonReleased));
		dp.Dispatch<MouseMovedEvent>(BIND_METHOD_1(ImGuiLayer::OnMouseMoved));
		dp.Dispatch<MouseScrolledEvent>(BIND_METHOD_1(ImGuiLayer::OnMouseScrolled));

		dp.Dispatch<KeyPressEvent>(BIND_METHOD_1(ImGuiLayer::OnKeyPressed));
		dp.Dispatch<KeyReleaseEvent>(BIND_METHOD_1(ImGuiLayer::OnKeyReleased));
		dp.Dispatch<KeyTypeEvent>(BIND_METHOD_1(ImGuiLayer::OnKeyTyped));

		dp.Dispatch<WindowResizeEvent>(BIND_METHOD_1(ImGuiLayer::OnWindowResize));

	}

	bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		ImGui::GetIO().MouseDown[e.GetMouseButton()] = true;
		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
		ImGui::GetIO().MouseDown[e.GetMouseButton()] = false;
		return false;
	}
	bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e) {
		ImGui::GetIO().MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}
	bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e) {
		ImGui::GetIO().MouseWheelH += e.GetXOffset();
		ImGui::GetIO().MouseWheel += e.GetYOffset();
		return false;
	}
	bool ImGuiLayer::OnKeyPressed(KeyPressEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	}
	bool ImGuiLayer::OnKeyReleased(KeyReleaseEvent& e) {
		ImGui::GetIO().KeysDown[e.GetKeyCode()] = false;
		return false;
	}
	bool ImGuiLayer::OnKeyTyped(KeyTypeEvent& e){
		unsigned char c = e.GetKeyCode();
		if (c > 0 && c < 0x10000) ImGui::GetIO().AddInputCharacter((GLushort)c);
		return false;
	}

	bool ImGuiLayer::OnWindowResize(WindowResizeEvent& e) {
		ImGui::GetIO().DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}