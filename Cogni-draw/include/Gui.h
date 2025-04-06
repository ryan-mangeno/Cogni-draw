#ifndef GUI_H
#define GUI_H

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class Gui
{
public:

	Gui(GLFWwindow*);
	~Gui();
	void render(uint32_t paint_fbo_id);


private:

	GLFWwindow* m_Window;
	ImGuiIO* m_Io;

	bool m_show_demo_window = true;
	bool m_show_another_window = false;
	const ImVec4 m_clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);


};


#endif
