#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawDock.h"


class App
{
public:
	App() = default;
	~App();

	bool init();
	void run();
	
private:

	void set_draw_dock(DrawDock& dock);

	GLFWwindow* m_Window;
};



#endif