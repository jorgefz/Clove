#include "clovepch.h"
#include "Clove/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "Clove/ImGui/ImGuiLayer.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Clove/Core/GameApp.h"

namespace Clove {

	ImGuiLayer::ImGuiLayer()
		: Layer()
	{

	}
	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::OnAttach() {
        CLOVE_PROFILE_FUNCTION();

        /*
        * COPIED FROM "examples/example_glfw_opengl3/main.cpp -> main()"
        */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

		GLFWwindow* window = static_cast<GLFWwindow*>(GameApp::Get().GetWindow().GetHandle());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

    void ImGuiLayer::OnDetach() {
        CLOVE_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& e) {
        if (!m_block_events) return;

        ImGuiIO& io = ImGui::GetIO();
        e.handled |= e.IsInGroup(EventGroupMouse) & io.WantCaptureMouse;
        e.handled |= e.IsInGroup(EventGroupKeyboard) & io.WantCaptureKeyboard;

    }

    void ImGuiLayer::Begin() {
        CLOVE_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        CLOVE_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();
        GameApp& app = GameApp::Get();
        io.DisplaySize = ImVec2(
            static_cast<float>(app.GetWindow().GetWidth()), 
            static_cast<float>(app.GetWindow().GetHeight())
        );

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender() {
        //static bool show = true;
        //ImGui::ShowDemoWindow(&show);

    }
}