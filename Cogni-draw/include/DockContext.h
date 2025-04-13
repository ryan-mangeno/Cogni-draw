#ifndef DOCK_CONTEXT_H
#define DOCK_CONTEXT_H


#include "DrawDock.h"
#include "ModelDock.h"


class DockContext
{
	friend class App;
public:

	DockContext() = default;

	inline void set_focus(Focus focus) { m_focused_dock = focus; }
	inline Focus get_focus() const { return m_focused_dock; }

	inline DrawDock* get_draw_dock() { return m_draw_dock; }
	inline ModelDock* get_model_dock() { return m_model_dock; }

private:

	explicit DockContext(DrawDock* drawDock, ModelDock* modelDock)
		: m_draw_dock(drawDock), m_model_dock(modelDock), m_focused_dock(Focus::NONE)
	{
	}

	DrawDock* m_draw_dock;
	ModelDock* m_model_dock;

	Focus m_focused_dock;
};


#endif