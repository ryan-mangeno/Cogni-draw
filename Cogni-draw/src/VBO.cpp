#include"VBO.h"

#include <iostream>
//temp
constexpr uint32_t width = 1920;
constexpr uint32_t height = 1080;


VBO::VBO(uint32_t size)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


VBO::VBO()
{
	glGenBuffers(1, &m_ID);
}

void VBO::buffer_data(std::vector<Vertex2D>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex2D) * vertices.size(), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void VBO::buffer_data(std::array<glm::vec2, 6>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * vertices.size(), vertices.data());
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