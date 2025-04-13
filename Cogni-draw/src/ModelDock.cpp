#include "ModelDock.h"


ModelDock::ModelDock(uint32_t width, uint32_t height)
	: m_Shader("Resources/Shaders/model_viewer.glsl"), m_Fbo({width, height, 1}), m_Camera(&Camera::GetCamera()), m_IsHovered(false)
{
}

ModelDock::~ModelDock()
{

}


void ModelDock::render()
{

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    m_Fbo.bind();

    m_Fbo.clear_fbo();

    const glm::uvec2& vp = m_Fbo.get_spec().viewport;
    glViewport(0, 0, vp[0], vp[1]);

    m_Shader.bind();

	for (auto& model : m_Models)
	{
		model.render(&m_Shader);
	}

    m_Fbo.unbind();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

}

void ModelDock::update(Focus focus_state, float deltaTime)
{
	m_IsHovered = focus_state == Focus::MODEL ? true : false;

    m_Camera->OnUpdate(deltaTime);
    m_Shader.set_uniform_mat4f("u_MVP", m_Camera->CalcMVP());

}

void ModelDock::push_model(const std::string& model_path)
{
	m_Models.emplace_back(model_path);
}