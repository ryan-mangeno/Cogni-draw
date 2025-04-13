#ifndef MODEL_DOCK_H
#define MODEL_DOCK_H


#include "Camera.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Model.h"

class ModelDock
{
public:

	ModelDock(uint32_t width, uint32_t height);
	~ModelDock();

	void render();
	void update(Focus focus_state, float deltaTime);


	void push_model(const std::string& model_path);
	
	inline Camera* get_camera() { return m_Camera; }
	inline uint32_t get_fbo_scene_ID() { return m_Fbo.get_color_attach(0); }
	inline bool get_hover_state() const { return m_IsHovered; }

private:

	FrameBuffer m_Fbo;
	Shader m_Shader;

	std::vector<::ModelLoader::Model> m_Models;

	bool m_IsHovered;
	Camera* m_Camera;
	
};



#endif