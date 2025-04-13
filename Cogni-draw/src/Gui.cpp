#include "Gui.h"

Gui::Gui(GLFWwindow* window) : m_Window(window)
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
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

}

Gui::~Gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::render(DrawDock& draw_dock, ModelDock& model_dock)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	m_focus = Focus::NONE;

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport());

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver); 
	ImGui::Begin("CG-Sketch");
	ImGui::Image(draw_dock.get_fbo_scene_ID(), ImVec2(1920, 1080));
	if (ImGui::IsItemHovered()) m_focus = Focus::DRAW;


	ImGui::End();


	ImGui::Begin("Model Viewer");
	ImGui::Image(model_dock.get_fbo_scene_ID(), ImVec2(1920, 1080));
	if (ImGui::IsItemHovered()) m_focus = Focus::MODEL;

	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_FirstUseEver);  
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver); 
	ImGui::Begin("Stencil Config", nullptr, ImGuiWindowFlags_NoCollapse);

	float stencil_config_size = draw_dock.get_stencil_size_ref();
	if (ImGui::SliderFloat("Stencil Size", &stencil_config_size, 0.0f, 20.0f, "%.2f"))
	{
		draw_dock.set_stencil_size(stencil_config_size);
	}

	// white
	static ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (ImGui::ColorEdit4("Stencil Color", (float*)&color))
	{
		draw_dock.set_stencil_color(glm::vec4{ color.x, color.y, color.z, color.w });
	}

	ImGui::End();

	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(m_Window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
