#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawDock.h"
#include "ModelDock.h"
#include "util.h"



class App
{
public:


	class DockContext
	{
		friend class App;
	public:

		DockContext() = default;

		inline void set_focus(Focus focus) { focused_dock = focus; }
		inline Focus get_focus() const { return focused_dock; }

		inline DrawDock* get_draw_dock() { return draw_dock; }
		inline ModelDock* get_model_dock() { return model_dock; }

	private:

		explicit DockContext(DrawDock* drawDock, ModelDock* modelDock)
			: draw_dock(drawDock), model_dock(modelDock)
		{
		}

		DrawDock* draw_dock = nullptr;
		ModelDock* model_dock = nullptr;

		Focus focused_dock = Focus::NONE;
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