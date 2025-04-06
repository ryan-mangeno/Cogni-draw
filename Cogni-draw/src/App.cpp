#include "App.h"
#include "Gui.h"
#include "DrawDock.h"

#include <iostream>

void App::run()
{

	if (!init()) return;

	Gui gui(m_Window);
	DrawDock paint(1920, 1080, "Resources/Shaders/2dpaint.glsl");

	while (!glfwWindowShouldClose(m_Window))
	{
		paint.update(m_Window);
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

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CW);
		//glEnable(GL_DEPTH_TEST);
	}

	return success;
}

