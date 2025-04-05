#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class App
{
public:
	App() = default;
	~App();

	bool init();
	void run();
	
private:
	GLFWwindow* m_Window;
};



#endif