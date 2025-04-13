#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "util.h"
#include "DockContext.h"


class App
{
public:


	struct AppAttribs
	{
	
		bool is_escaped = true;
		bool first_mouse = true;

		static AppAttribs& get_app_attribs()
		{
			static AppAttribs attribs;
			return attribs;
		}

	private:
		AppAttribs() = default;

		AppAttribs(const AppAttribs&) = delete;
		AppAttribs& operator=(const AppAttribs&) = delete;

	};

	App() = default;
	~App();

	bool init();
	void run();
	
private:

	void set_draw_dock(DrawDock& drawDock, ModelDock& modelDock);

	DockContext m_DockContext;
	GLFWwindow* m_Window;
};



#endif