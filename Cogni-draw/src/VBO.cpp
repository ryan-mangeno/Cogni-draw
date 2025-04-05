#include"VBO.h"
#include <vector>


VBO::VBO(std::vector<Vertex>& vertices, uint32_t count)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count, nullptr, GL_DYNAMIC_DRAW);
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