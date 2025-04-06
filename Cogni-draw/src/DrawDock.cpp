#include "DrawDock.h"


// for right now the frame buffer will be the same size at the drawing pad
DrawDock::DrawDock(unsigned int width, unsigned int height, const std::string& shader_path)
	: m_Fbo({ width, height }), m_Shader(shader_path), m_Width(width), m_Height(height), m_Vao(), m_Vbo()
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
    glDrawArrays(GL_POINTS, 0, m_DrawnVertices.size());
    m_Vao.unbind();

    m_Fbo.unbind();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]); 

}

// to make callback for 
void DrawDock::on_click_or_drag(float mouse_x, float mouse_y)
{
    float normalizedX = mouse_x / m_Width;
    float normalizedY = 1.0f - (mouse_x / m_Height);

    m_DrawnVertices.push_back({ glm::vec2(normalizedX, normalizedY), {1.0f, 0.0f, 0.0f} });
    m_Vbo.buffer_data(m_DrawnVertices);
}


// temporary
void DrawDock::update(GLFWwindow* window)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // normalizing
    float normX = static_cast<float>(mouseX) / width;
    float normY = static_cast<float>(mouseY) / height;

    // convert [0, 1] to NDC [-1, 1]
    float ndcX = (2.0f * normX) - 1.0f;  // Maps [0, 1] to [-1, 1]
    float ndcY = (2.0f * normY) - 1.0f;  // Maps [0, 1] to [-1, 1]

    // NDC goes from -1 to 1 for both axes, so check this before adding
    if (ndcX >= -1.0f && ndcX <= 1.0f && ndcY >= -1.0f && ndcY <= 1.0f)
    {
        glm::vec2 pos(ndcX, ndcY);
        glm::vec3 color(1.0f, 0.0f, 0.0f);

        std::cout << "NDC Position: (" << pos.x << ", " << pos.y << ") " << "Vertex Count: " << m_DrawnVertices.size() << std::endl;
        m_DrawnVertices.push_back({ pos, color });

        m_Vbo.buffer_data(m_DrawnVertices);
    }
}