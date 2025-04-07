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
	DrawDock(unsigned int width, unsigned int height, const std::string& shader_path);
	~DrawDock();

	void render();
	void update(bool hover_state);

	void draw(float mouse_x, float mouse_y);
	void stop_draw();
	void copy(float mouse_x, float mouse_y);
	void end_copy();


	inline uint32_t get_fbo_scene_ID() { return m_Fbo.get_scene_tex_ID(); }
	inline bool get_hover_state() const { return m_IsHovered; }

private:
	Shader m_Shader;
	FrameBuffer m_Fbo;
	VAO m_Vao;
	VBO m_Vbo;
	uint32_t m_CanvasTextureID;
	std::vector<Vertex2D> m_DrawnVertices; 
	std::vector<uint32_t> m_Delimters; // when we pick up stencil, we know where to draw to

	unsigned int m_Height, m_Width;
	bool m_IsHovered;
};


#endif
