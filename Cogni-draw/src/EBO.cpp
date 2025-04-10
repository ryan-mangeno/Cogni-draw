#include "EBO.h"


EBO::EBO()
{
	glGenBuffers(1, &m_ID); 
}


EBO::EBO(const uint32_t* indices, uint32_t count)
{
    glGenBuffers(1, &m_ID);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
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

