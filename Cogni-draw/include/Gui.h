#ifndef GUI_H
#define GUI_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "DrawDock.h"
#include "ModelDock.h"

class Gui
{
public:

	Gui(GLFWwindow*);
	~Gui();
	void render(DrawDock& draw_dock, ModelDock& model_dock);

	inline Focus get_focus_state() { return m_focus; }

private:

	GLFWwindow* m_Window;
	ImGuiIO* m_Io;

	bool m_show_demo_window = true;
	bool m_show_another_window = false;
	Focus m_focus;
	const ImVec4 m_clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);


};


#endif
