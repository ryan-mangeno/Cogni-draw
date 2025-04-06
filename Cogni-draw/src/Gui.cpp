#include "Gui.h"

Gui::Gui(GLFWwindow* window) : m_Window(window), m_IsHovered(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_Io = &ImGui::GetIO();

	// enabling some flags

	m_Io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
	m_Io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; 


	ImGui::StyleColorsDark();

	// setting up Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);

}

Gui::~Gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::render(uint32_t paint_fbo_id)
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	// docking window
	ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport());

	// position the window
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver); 

	ImGui::Begin("-~+");
	ImGui::Image(paint_fbo_id, ImVec2(1920, 1080));
	m_IsHovered = ImGui::IsItemHovered();

	ImGui::End();

	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(m_Window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
