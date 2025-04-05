#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"


class VAO
{
public:

	VAO();
	~VAO();

	void link_attrib(VBO& VBO, unsigned int layout, int numComponents, GLenum type, int stride, void* offset);
	void bind() const;
	void unbind() const;

	inline uint32_t get_ID() const { return m_ID; }

private:
	uint32_t m_ID;

};

#endif