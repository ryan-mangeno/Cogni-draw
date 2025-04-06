#include "EBO.h"





EBO::EBO(uint32_t* indices, uint32_t count)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * count , indices);

}

EBO::~EBO()
{
	glDeleteBuffers(1, &m_ID);
}

void EBO::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void EBO::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

