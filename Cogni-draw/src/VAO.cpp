#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &m_ID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

// links a VBO to the VAO using a certain layout
void VAO::link_attrib(VBO& VBO, unsigned int layout, int numComponents, GLenum type, int stride, void* offset)
{
	bind();
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.unbind();
	unbind();
}

void VAO::bind() const
{
	glBindVertexArray(m_ID);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

