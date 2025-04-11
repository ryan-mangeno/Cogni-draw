#include "DrawDock.h"
#include "mathutil.h"



// for right now the frame buffer will be the same size at the drawing pad
DrawDock::DrawDock(unsigned int width, unsigned int height)
	: m_Fbo({ width, height, 2 }), m_PaintShader("Resources/Shaders/2dpaint.glsl"), m_CopyShader("Resources/Shaders/drag_rect.glsl"),
    m_Width(width), m_Height(height), m_PaintVao(), m_PaintVbo(sizeof(Vertex2D) * width * height), m_CopyVao(), m_CopyVbo(sizeof(glm::vec2)*6), m_IsHovered(false),
    m_IsDrawing(false), m_IsCopying(false), m_CopyRect(), m_StencilSize(3.05f), m_Color(1.0f, 1.0f, 1.0f)
{
    m_PaintVao.link_attrib(m_PaintVbo, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
    m_PaintVao.link_attrib(m_PaintVbo, 1, 3, GL_FLOAT, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));

    m_CopyVao.link_attrib(m_CopyVbo, 0, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);

    m_Fbo.clear_fbo();
}

DrawDock::~DrawDock()
{

}

void DrawDock::update(bool hover_state)
{
    m_IsHovered = hover_state;
}

void DrawDock::render()
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    m_Fbo.bind();
    m_Fbo.set_draw_target(0);

    const glm::uvec2& vp = m_Fbo.get_spec().viewport;
    glViewport(0, 0, vp[0], vp[1]);

    m_PaintShader.bind();

    glLineWidth(m_StencilSize);

    m_PaintVao.bind();

    // drawing the current brush of paint 
    glDrawArrays(GL_LINE_STRIP, 0, m_DrawnVertices.size());

    m_Fbo.set_draw_target(1);
    m_Fbo.clear_fbo_target(1);
    m_Fbo.blit_buffer();

    if (m_IsCopying)
    {
        // drawing to copy color attachment buffer which is at index 1, i will add macros soon
        m_CopyShader.bind();
        m_CopyVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        m_CopyVao.unbind();
    }
    
    m_Fbo.unbind();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]); 

}

void DrawDock::start_draw(float mouse_x, float mouse_y)
{

    if (!m_IsHovered)
        return;
    
    m_IsCopying = false;
    m_IsDrawing = true;

    // transform into ndc cords
    float normalizedX = (2.0f * mouse_x) / m_Width - 1.0f;  
    float normalizedY = -(1.0f - (2.0f * mouse_y) / m_Height);


    m_DrawnVertices.push_back({ glm::vec2(normalizedX, normalizedY), m_Color });
    m_PaintVbo.buffer_data(m_DrawnVertices);
}


void DrawDock::stop_draw()
{
    if (!m_IsHovered)
        return;

    m_IsDrawing = false;

    // since we rendered to fbo we dont have to clear unless the user wants to, we can simple flush vertices
    std::vector<Vertex2D>().swap(m_DrawnVertices);
}


void DrawDock::start_copy(float mouse_x, float mouse_y)
{
    if (!m_IsHovered)
        return;

    m_CopyPivot = { mouse_x, mouse_y };
    m_IsCopying = true;
    m_IsDrawing = false;

}

void DrawDock::end_copy()
{
    if (!m_IsHovered)
        return;

    reset_rect();
    m_CopyVbo.buffer_data(m_CopyRect);

    m_IsCopying = false;
}

void DrawDock::on_cursor_move(float mouse_x, float mouse_y)
{
    if (m_IsCopying)
    {
        update_copy_buffer(mouse_x, mouse_y);
    }
    else if (m_IsDrawing)
    {
        start_draw(mouse_x, mouse_y);
    }
}


void DrawDock::update_copy_buffer(float cur_mouse_x, float cur_mouse_y)
{
    glm::vec2 p = m_CopyPivot;
    glm::vec2 e = { cur_mouse_x, cur_mouse_y };
    glm::vec2 c1 = { e.x, p.y };
    glm::vec2 c2 = { p.x, e.y };

    auto toNDC = [&](const glm::vec2& px) {
        return glm::vec2(
            2.0f * px.x / m_Width - 1.0f,
            -(1.0f - 2.0f * px.y / m_Height)
        );
        };

    glm::vec2 pN = toNDC(p);
    glm::vec2 c1N = toNDC(c1);
    glm::vec2 c2N = toNDC(c2);
    glm::vec2 eN = toNDC(e);

    m_CopyRect[0] = pN;
    m_CopyRect[1] = c1N;
    m_CopyRect[2] = c2N;
    m_CopyRect[3] = c2N;
    m_CopyRect[4] = c1N;
    m_CopyRect[5] = eN;

    m_CopyVbo.buffer_data(m_CopyRect);
}