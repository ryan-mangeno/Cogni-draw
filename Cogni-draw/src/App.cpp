#include "App.h"
#include "Gui.h"
#include "DrawDock.h"

#include <iostream>

void App::run()
{

	if (!init()) return;

	Gui gui(m_Window);
	DrawDock paint(1920, 1080, "Resources/Shaders/2dpaint.glsl");
	set_draw_dock(paint);

	while (!glfwWindowShouldClose(m_Window))
	{
		paint.update(gui.get_hover_state());
		paint.render();
		gui.render(paint.get_fbo_scene_ID());
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
}


App::~App()
{

	// delete shaders and textures when i get there

	glfwDestroyWindow(m_Window);
	m_Window = nullptr;

	glfwTerminate();
}





static bool mouse_held = false;


// need to make call back for keyboard so when you copy you hit enter to make api call

static void on_mouse_click(GLFWwindow* window, int button, int action, int mods)
{

	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

	// for drawing if in 2d
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		DrawDock* dock = static_cast<DrawDock*>(glfwGetWindowUserPointer(window));
		if (action == GLFW_PRESS)
		{
			mouse_held = true;
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			dock->draw(static_cast<float>(x), static_cast<float>(y));
		}
		else if (action == GLFW_RELEASE)
		{
			dock->on_draw_release();
			mouse_held = false;
		}
	}
	// for drag and copy, once again in 2d for now
	else if( button == GLFW_MOUSE_BUTTON_RIGHT )
	{ 
		DrawDock* dock =static_cast<DrawDock*>(glfwGetWindowUserPointer(window));
	 	if(action == GLFW_PRESS)
		{
			mouse_held = true;
			double x,y;
			glfwGetCursorPos(window, &x, &y);
			dock->start_copy(static_cast<float>(x), static_cast<float>(y));
		}
		else if( action == GLFW_RELEASE)
		{
			dock->stop_copy();
			mouse_held = false;
		}

	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

	if (mouse_held)
	{
		auto* dock = static_cast<DrawDock*>(glfwGetWindowUserPointer(window));
		dock->on_click_or_drag(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}



void App::set_draw_dock(DrawDock& dock) 
{
	glfwSetWindowUserPointer(m_Window, &dock);
	glfwSetMouseButtonCallback(m_Window, on_mouse_click);
	glfwSetCursorPosCallback(m_Window, cursor_position_callback);
}



bool App::init()
{
	bool success = true;


	constexpr uint32_t SCREEN_WIDTH = 1920;
	constexpr uint32_t SCREEN_HEIGHT = 1080;

	// init GLFW
	if (!glfwInit())
		success = false;

	else 
	{

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create window
		m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cogni-Draw", NULL, NULL);
		if (m_Window == NULL)
		{
			std::cout << "Failed to create GLFW window\n";
			glfwTerminate();
			success = false;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD\n";
			success = false;
		}

		// Configure viewport and rendering
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		
		// need to set these
		
		//glfwSetFramebufferSizeCallback(m_Window, CallBackStates::framebufferSizeCallback);
		//glfwSetCursorPosCallback(m_Window, CallBackStates::mouse_callback);
		//glfwSetScrollCallback(m_Window, CallBackStates::scroll_callback);

		// white
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA); // might need to enable/disable when rendering 2d/3d
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CW);
		//glEnable(GL_DEPTH_TEST);
	}

	return success;
}

