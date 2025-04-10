#include "DrawDock.h"
#include "mathutil.h"



// for right now the frame buffer will be the same size at the drawing pad
DrawDock::DrawDock(unsigned int width, unsigned int height)
	: m_Fbo({ width, height }), m_PaintShader("Resources/Shaders/2dpaint.glsl"), m_CopyShader("Resources/Shaders/drag_rect.glsl"),
    m_Width(width), m_Height(height), m_PaintVao(), m_PaintVbo(sizeof(Vertex2D) * width * height), m_CopyVao(), m_CopyVbo(sizeof(glm::vec2)*6), m_IsHovered(false),
    m_IsDrawing(false), m_IsCopying(false), m_CopyRect()
{
    m_PaintVao.link_attrib(m_PaintVbo, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
    m_PaintVao.link_attrib(m_PaintVbo, 1, 3, GL_FLOAT, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));

    m_CopyVao.link_attrib(m_CopyVbo, 0, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);
    

    // will be drawing on this texture
    glGenTextures(1, &m_CanvasTextureID); 
    glBindTexture(GL_TEXTURE_2D, m_CanvasTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

    // empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); 
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

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::uvec2& vp = m_Fbo.get_spec().viewport;
    glViewport(0, 0, vp[0], vp[1]);

    m_PaintShader.bind();
    glBindTexture(GL_TEXTURE_2D, m_CanvasTextureID);

    glPointSize(10.0f);
    m_PaintVao.bind();


    if (m_Delimters.size() > 0 && m_DrawnVertices.size() > 0)
    {

        size_t startIdx = 0; 
        size_t endIdx = 0;
        for (size_t i = 0; i < m_Delimters.size(); ++i)
        {
            size_t endIdx = m_Delimters[i]; 

            // if start is on a delimeter just advance is up 1
            if (startIdx != 0)startIdx++;

            // draw between the start and end indices
            if (startIdx < endIdx) 
            {
                glDrawArrays(GL_LINE_STRIP, startIdx, endIdx - startIdx);
            }

            startIdx = endIdx;
        }
        
        // if we are currently drawing there wont be an end delimeter yet so just draw until the end of drawn verts
        if (m_Delimters[m_Delimters.size() - 1] != m_DrawnVertices.size() - 1)
        {
            endIdx = m_DrawnVertices.size() - 1;
            glDrawArrays(GL_LINE_STRIP, startIdx+1, endIdx - startIdx);
        }
    }
    else
    {
        glDrawArrays(GL_LINE_STRIP, 0, m_DrawnVertices.size());
    }

    
    if (m_IsCopying)
    {
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


    m_DrawnVertices.push_back({ glm::vec2(normalizedX, normalizedY), {1.0f, 0.0f, 0.0f} });
    m_PaintVbo.buffer_data(m_DrawnVertices);
}


void DrawDock::stop_draw()
{
    if (!m_IsHovered)
        return;

    m_IsDrawing = false;

    m_Delimters.push_back(m_DrawnVertices.size() - 1);
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