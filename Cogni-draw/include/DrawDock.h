#ifndef DRAW_DOCK_H
#define DRAW_DOCK_H


#include "FrameBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"

#include <glfw/glfw3.h>

class DrawDock
{

public:
	DrawDock(unsigned int width, unsigned int height);
	~DrawDock();

	void render();
	void update(bool hover_state);

	void start_draw(float mouse_x, float mouse_y);
	void stop_draw();
	void start_copy(float mouse_x, float mouse_y);
	void end_copy();

	inline float& get_stencil_size_ref() { return m_StencilSize; }
	void on_cursor_move(float mouse_x, float mouse_y);
	inline void set_stencil_size(float size) { m_StencilSize = size; }
	inline void set_stencil_color(glm::vec3 color) { m_Color = color; }

	inline uint32_t get_fbo_scene_ID() { return m_Fbo.get_full_scene_tex_ID(); }
	inline bool get_hover_state() const { return m_IsHovered; }

private:

	void update_copy_buffer(float cur_mouse_x, float cur_mouse_y);
	inline void reset_rect() 
	{
		std::fill(std::begin(m_CopyRect), std::end(m_CopyRect), glm::vec2(0.0f, 0.0f));
	}

private:

	Shader m_PaintShader;
	Shader m_CopyShader;

	FrameBuffer m_Fbo;

	VAO m_PaintVao;
	VAO m_CopyVao;

	VBO m_PaintVbo;
	VBO m_CopyVbo;

	std::vector<Vertex2D> m_DrawnVertices; 

	std::array<glm::vec2, 6> m_CopyRect;
	glm::vec2 m_CopyPivot;

	unsigned int m_Height, m_Width;
	bool m_IsHovered;
	bool m_IsDrawing;
	bool m_IsCopying;
	float m_StencilSize;
	glm::vec3 m_Color;
};


#endif
