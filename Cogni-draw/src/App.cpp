#include "App.h"
#include "Gui.h"
#include "DrawDock.h"
#include "RunPy.h"

#include <iostream>

static float deltaTime = 0.0f;

void App::run()
{

	if (!init()) return;

	Gui gui(m_Window);
	DrawDock paint(1920, 1080);
	ModelDock model_viewer(1920, 1080);
	set_draw_dock(paint, model_viewer);

	std::atomic_flag is_done;
	run_async_python("python/anthro_api.py", is_done);

	model_viewer.push_model("Resources/Assets/ghost.obj");


	float prevTime = 0.0f;
	float curTime = 0.0f;

	while (!glfwWindowShouldClose(m_Window))
	{

		prevTime = curTime;
		curTime = glfwGetTime();
		deltaTime = curTime - prevTime;

		paint.update(gui.get_focus_state());
		model_viewer.update(gui.get_focus_state(), deltaTime);
		m_DockContext.set_focus(gui.get_focus_state());
		
		gui.render(paint, model_viewer);
		paint.render();
		model_viewer.render();

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
}


App::~App()
{
	glfwDestroyWindow(m_Window);
	m_Window = nullptr;

	glfwTerminate();
}


static void on_mouse_click(GLFWwindow* window, int button, int action, int mods)
{

	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);


	App::DockContext* dock_context = static_cast<App::DockContext*>(glfwGetWindowUserPointer(window));

	Focus cur_focus = dock_context->get_focus();

	if (cur_focus == Focus::DRAW)
	{
		DrawDock* dock = dock_context->get_draw_dock();

		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				dock->start_draw(static_cast<float>(x), static_cast<float>(y));
			}
			else if (action == GLFW_RELEASE)
			{
				dock->stop_draw();
			}
		}

		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (action == GLFW_PRESS)
			{
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				dock->start_copy(static_cast<float>(x), static_cast<float>(y));
			}
			else if (action == GLFW_RELEASE)
			{
				dock->end_copy();
			}
		}
	}
	
}

static bool escaped = false;


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	App::DockContext* dock_context = static_cast<App::DockContext*>(glfwGetWindowUserPointer(window));

	Focus cur_focus = dock_context->get_focus();


	if (cur_focus == Focus::DRAW)
	{
		DrawDock* draw_dock = dock_context->get_draw_dock();
		draw_dock->on_cursor_move(static_cast<float>(xpos), static_cast<float>(ypos));
	}
	else if (cur_focus == Focus::MODEL)
	{
		ModelDock* dock = dock_context->get_model_dock();

		Camera* cam = dock->get_camera();
		static bool first_mouse = false;
		static double prev_mouse_x, prev_mouse_y;

		if (escaped)
			return;

		int win_width, win_height;
		glfwGetWindowSize(window, &win_width, &win_height);
		double center_x = win_width / 2.0;
		double center_y = win_height / 2.0;

		if (first_mouse) 
		{
			glfwSetCursorPos(window, center_x, center_y);
			prev_mouse_x = center_x;
			prev_mouse_y = center_y;
			first_mouse = false;
		}

		float xoffset = xpos - prev_mouse_x;
		float yoffset = ypos - prev_mouse_y;

		prev_mouse_x = xpos;
		prev_mouse_y = ypos;

		cam->DispatchMouseMoveEvent(xoffset, yoffset);

		glfwSetCursorPos(window, center_x, center_y);
		prev_mouse_x = center_x;
		prev_mouse_y = center_y;
	}
}


static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	App::DockContext* dock_context = static_cast<App::DockContext*>(glfwGetWindowUserPointer(window));

	Focus cur_focus = dock_context->get_focus();


	if (cur_focus == Focus::DRAW)
	{
		//DrawDock* draw_dock = dock_context->get_draw_dock();
	}


	else if (cur_focus == Focus::MODEL)
	{
		ModelDock* dock = dock_context->get_model_dock();
		Camera* cam = dock->get_camera();

		auto key_to_dir = [](int key) -> MovementDir {
			switch (key)
			{
				case GLFW_KEY_A: return MovementDir::LEFT;
				case GLFW_KEY_D: return MovementDir::RIGHT;
				case GLFW_KEY_W: return MovementDir::FORWARD;
				case GLFW_KEY_S: return MovementDir::BACK;
				case GLFW_KEY_Q: return MovementDir::DOWN;
				case GLFW_KEY_E: return MovementDir::UP;
				case GLFW_KEY_ESCAPE: escaped = !escaped;
				default: return MovementDir::NONE;
			}
			};

		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			MovementDir dir = key_to_dir(key);
			if (dir != MovementDir::NONE)
			{
				cam->DispatchKeyboardEvent(dir, deltaTime); 
			}
		}
	}
}


void App::set_draw_dock(DrawDock& drawDock, ModelDock& modelDock) 
{
	m_DockContext = DockContext(&drawDock, &modelDock);

	glfwSetWindowUserPointer(m_Window, &m_DockContext);
	glfwSetMouseButtonCallback(m_Window, on_mouse_click);
	glfwSetCursorPosCallback(m_Window, cursor_position_callback);
	glfwSetKeyCallback(m_Window, keyboard_callback);
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

