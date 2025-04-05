#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <vector>

#include "util.h"

class VBO
{
public:	
	

	VBO(std::vector<Vertex>& vertices, uint32_t count);
	~VBO();

	void bind() const;
	void unbind() const;

	inline uint32_t get_ID() const { return m_ID; }

private:
	uint32_t m_ID;


};


#endif