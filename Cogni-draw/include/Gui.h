#ifndef GUI_H
#define GUI_H

#include "DrawDock.h"


#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class Gui
{
public:

	Gui(GLFWwindow*);
	~Gui();
	void render(DrawDock& dock);

	inline bool get_hover_state() const { return m_IsHovered; }

private:

	GLFWwindow* m_Window;
	ImGuiIO* m_Io;

	bool m_show_demo_window = true;
	bool m_show_another_window = false;
	bool m_IsHovered;
	const ImVec4 m_clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);


};


#endif
