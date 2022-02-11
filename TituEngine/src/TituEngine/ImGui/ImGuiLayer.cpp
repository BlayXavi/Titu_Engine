#include "tepch.h"
#include "ImGuiLayer.h"
#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "TituEngine/Core/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

namespace TituEngine
{

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::SetDefaultTheme()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.11f, 0.11f, 0.11f, 0.71f);
		colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.33f, 0.33f, 0.33f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.82f, 0.82f, 0.82f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 0.45f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.69f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 0.66f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		
		ImGuiStyle& style = ImGui::GetStyle();

		style.PopupRounding = 3;

		style.WindowPadding = ImVec2(2, 2);
		style.FramePadding = ImVec2(6, 4);
		style.ItemSpacing = ImVec2(6, 2);
		style.ItemInnerSpacing = ImVec2(4, 4);
		style.TouchExtraPadding = ImVec2(0, 0);

		style.IndentSpacing = 26;
		style.ScrollbarSize = 14;

		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 1;

		style.WindowRounding = 0;
		style.ChildRounding = 1;
		style.FrameRounding = 1;
		style.PopupRounding = 1;
		style.ScrollbarRounding = 1;
		style.GrabRounding = 1;
		style.LogSliderDeadzone = 1;
	}

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
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
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		SetDefaultTheme();

		io.Fonts->AddFontFromFileTTF("resources/fonts/ubuntu/Ubuntu-Bold.ttf", 14.0f);
		io.Fonts->AddFontFromFileTTF("resources/fonts/ubuntu/Ubuntu-Italic.ttf", 14.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/ubuntu/Ubuntu-Regular.ttf", 14.0f);

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::BeginRender()
	{
		TE_PROFILE_PROFILE_FUNC();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::EndRender()
	{
		TE_PROFILE_PROFILE_FUNC();
		ImGuiIO& io = ImGui::GetIO();

		{
			TE_PROFILE_PROFILE_SCOPE("Display Size");

			Application& app = Application::Instance();
			io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		}

		// Rendering
		{
			TE_PROFILE_PROFILE_SCOPE("Render");
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}


		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			TE_PROFILE_PROFILE_SCOPE("ViewPorts");
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		/*static bool show = false;
		ImGui::ShowDemoWindow(&show);*/

	}

}
