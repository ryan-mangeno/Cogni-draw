#include "DrawDock.h"


// for right now the frame buffer will be the same size at the drawing pad
DrawDock::DrawDock(unsigned int width, unsigned int height, const std::string& shader_path)
	: m_Fbo({ width, height }), m_Shader(shader_path), m_Width(width), m_Height(height), m_Vao(), m_Vbo(), m_IsHovered(false)
{

    m_Vao.link_attrib(m_Vbo, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
    m_Vao.link_attrib(m_Vbo, 1, 3, GL_FLOAT, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));


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

    const glm::uvec2& vp = m_Fbo.get_spec().viewport;
    glViewport(0, 0, vp[0], vp[1]);

    m_Shader.bind();
    glBindTexture(GL_TEXTURE_2D, m_CanvasTextureID);

    glPointSize(10.0f);
    m_Vao.bind();


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


    m_Vao.unbind();
    m_Fbo.unbind();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]); 

}

void DrawDock::start_draw(float mouse_x, float mouse_y)
{

    if (!m_IsHovered)
        return;

    // transform into ndc cords
    float normalizedX = (2.0f * mouse_x) / m_Width - 1.0f;  
    float normalizedY = -(1.0f - (2.0f * mouse_y) / m_Height);


    m_DrawnVertices.push_back({ glm::vec2(normalizedX, normalizedY), {1.0f, 0.0f, 0.0f} });
    m_Vbo.buffer_data(m_DrawnVertices);
}


void DrawDock::stop_draw()
{
    if (!m_IsHovered)
        return;

    m_Delimters.push_back(m_DrawnVertices.size() - 1);
}
