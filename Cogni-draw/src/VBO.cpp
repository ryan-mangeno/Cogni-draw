#include"VBO.h"
#include <vector>


//temp
constexpr uint32_t width = 1920;
constexpr uint32_t height = 1080;

VBO::VBO(std::vector<Vertex2D>& vertices)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * width * height, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex2D) * vertices.size(), vertices.data());
}

VBO::VBO()
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * width * height, nullptr, GL_DYNAMIC_DRAW);
}

void VBO::buffer_data(std::vector<Vertex2D>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex2D) * vertices.size(), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


VBO::~VBO()
{
	glDeleteBuffers(1, &m_ID);
}

void VBO::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VBO::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}