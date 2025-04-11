#include "App.h"
#include "Gui.h"
#include "DrawDock.h"
#include "RunPy.h"

#include <iostream>

void App::run()
{

	if (!init()) return;

	Gui gui(m_Window);
	DrawDock paint(1920, 1080);
	set_draw_dock(paint);

	std::atomic_flag is_done;
	run_async_python("python/anthro_api.py", is_done);

	while (!glfwWindowShouldClose(m_Window))
	{
		paint.update(gui.get_hover_state());
		paint.render();
		gui.render(paint);
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
			dock->start_draw(static_cast<float>(x), static_cast<float>(y));
		}
		else if (action == GLFW_RELEASE)
		{
			dock->stop_draw();
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
			dock->end_copy();
			mouse_held = false;
		}

	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	DrawDock* dock = static_cast<DrawDock*>(glfwGetWindowUserPointer(window));
	
	// add bool for drawing and copying
	// bool drawing = dock->get_draw_state();
	// bool copying = dock->get_copy_state();

	if (mouse_held)
	{
		DrawDock* dock = static_cast<DrawDock*>(glfwGetWindowUserPointer(window));
		dock->on_cursor_move(static_cast<float>(xpos), static_cast<float>(ypos));
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
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glEnable(GL_BLEND);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CW);
		//glEnable(GL_DEPTH_TEST);
	}

	return success;
}

