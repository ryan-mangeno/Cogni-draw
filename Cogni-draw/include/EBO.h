#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

// for uint32_t
#include <cstdint>   


class EBO
{

public:

	EBO(uint32_t* indices, uint32_t count);
	~EBO();

	void bind() const;
	void unbind() const;

	inline uint32_t get_ID() const { return m_ID; };

private:
	uint32_t m_ID;

};

#endif