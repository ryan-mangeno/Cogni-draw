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

	void update(GLFWwindow* window);
	void render();

	void on_click_or_drag(float mouse_x, float mouse_y);

	inline uint32_t get_fbo_scene_ID() { return m_Fbo.get_scene_tex_ID(); }

private:
	Shader m_Shader;
	FrameBuffer m_Fbo;
	VAO m_Vao;
	VBO m_Vbo;
	uint32_t m_CanvasTextureID;
	std::vector<Vertex2D> m_DrawnVertices; 

	unsigned int m_Height, m_Width;

};


#endif