#include "ModelDock.h"
#include "ModelBuffer.h"

ModelDock::ModelDock(uint32_t width, uint32_t height)
	: m_Shader("Cogni-draw/Resources/Shaders/model_viewer.glsl"), m_Fbo({width, height, 1}), m_Camera(&Camera::GetCamera()), m_IsHovered(false)
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
	m_Shader.set_uniform_vec3f("u_ViewPos", m_Camera->GetPos());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    ModelBuffer& mb = ModelBuffer::get_instance();
    if (mb.peek_buffer()) // if a model is done and ready to be loaded
    {
        mb.pop_model();
        push_model("Cogni-draw/Resources/Assets/generated_model.obj");
    }
}

void ModelDock::push_model(const std::string& model_path)
{
	m_Models.emplace_back(model_path);
}