#include "DrawDock.h"
#include "mathutil.h"
#include "Image.h"
#include <limits>
#include <algorithm>


// for right now the frame buffer will be the same size at the drawing pad
DrawDock::DrawDock(uint32_t width, uint32_t height)
	: m_Fbo({ width, height, 2 }), m_PaintShader("Resources/Shaders/2dpaint.glsl"), m_CopyShader("Resources/Shaders/drag_rect.glsl"),
    m_Width(width), m_Height(height), m_PaintVao(), m_PaintVbo(sizeof(Vertex2D) * width * height), m_CopyVao(), m_CopyVbo(sizeof(glm::vec2)*6), m_IsHovered(false),
    m_IsDrawing(false), m_IsCopying(false), m_CopyRect(), m_StencilSize(3.05f), m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
    m_PaintVao.link_attrib(m_PaintVbo, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
    m_PaintVao.link_attrib(m_PaintVbo, 1, 4, GL_FLOAT, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));

    m_CopyVao.link_attrib(m_CopyVbo, 0, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);

    m_Fbo.clear_fbo();
}

DrawDock::~DrawDock()
{

}

void DrawDock::update(Focus focus_state)
{
    m_IsHovered = focus_state == Focus::DRAW ? true : false;
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
    std::cout << m_DrawnVertices.size() << '\n';

    // since we rendered to fbo we dont have to clear unless the user wants to, we can simply flush vertices
    std::vector<Vertex2D>().swap(m_DrawnVertices);
    // could use clear and shrink to fit but shrink to fit isnt garunteed to be fulfilled
    // and while clear is useful here since vertex2d is trivial, I don't want the memory to persist since users can make draws very long
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

    copy_fbo_rect();
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

    const glm::vec2& vp = m_Fbo.get_spec().viewport;

    glm::vec2 pN = pixel_to_ndc(p, vp.x, vp.y); // pivot corner
    glm::vec2 c1N = pixel_to_ndc(c1, vp.x, vp.y); // above/below mouse 
    glm::vec2 c2N = pixel_to_ndc(c2, vp.x, vp.y); // above/below pivot
    glm::vec2 eN = pixel_to_ndc(e, vp.x, vp.y); // mouse corner


    // since we all glDrawArrays(...) we ensure that indices don't matter when drawing the quad
    
    // first tri, using pivot and 2 corners that are not the mouse corner
    m_CopyRect[0] = pN;
    m_CopyRect[1] = c1N;
    m_CopyRect[2] = c2N;

    // second tri, using the 2 corners and the mouse corner
    m_CopyRect[3] = c2N;
    m_CopyRect[4] = c1N;
    m_CopyRect[5] = eN;

    m_CopyVbo.buffer_data(m_CopyRect);
}

void DrawDock::copy_fbo_rect()
{

    constexpr size_t n = std::tuple_size<decltype(m_CopyRect)>::value;

    // bottom left is min x min y, top right is max x max y, bl is index 0, tr is index 1
    std::array<glm::vec2, 2> fbo_copy_rect = {};

    float min_x = std::numeric_limits<float>::infinity();    
    float min_y = std::numeric_limits<float>::infinity();  
    float max_x = -std::numeric_limits<float>::infinity();   
    float max_y = -std::numeric_limits<float>::infinity();   

    auto minmax_x = std::minmax_element(m_CopyRect.begin(), m_CopyRect.end(),
        [](const glm::vec2& a, const glm::vec2& b){
            return a.x < b.x;  
        });

    // in ndc the y coords are flipped so we flip innequality to find the relative max y
    auto minmax_y = std::minmax_element(m_CopyRect.begin(), m_CopyRect.end(),
        [](const glm::vec2& a, const glm::vec2& b){
            return a.y > b.y;
        });

    const glm::vec2& viewport = m_Fbo.get_spec().viewport;

    fbo_copy_rect[0] = ndc_to_pixel({ minmax_x.first->x, minmax_y.first->y }, viewport.x, viewport.y);
    fbo_copy_rect[1] = ndc_to_pixel({ minmax_x.second->x, minmax_y.second->y }, viewport.x, viewport.y);

    int width = static_cast<int>(fbo_copy_rect[1].x - fbo_copy_rect[0].x);
    int height = static_cast<int>(fbo_copy_rect[1].y - fbo_copy_rect[0].y);


    m_Fbo.bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    // the image is upside down when its going to be read so we need to start reading from the y max and gets it flipped position
    int flipped_y =  static_cast<int>(viewport.y - fbo_copy_rect[1].y);

    // 4 for rgba
    unsigned char* bytes = new unsigned char[width * height * 4];
    glReadPixels(static_cast<int>(fbo_copy_rect[0].x), flipped_y,
        width, height, GL_RGBA, GL_UNSIGNED_BYTE, bytes); 


    save_png(bytes, width, height);
    delete[] bytes;


    m_Fbo.unbind();


}