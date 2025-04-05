
#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

// for uint32_t
#include <cstdint>   


class EBO
{

public:

	EBO(uint32_t indices[], uint32_t count);
	~EBO();

	void Bind() const;
	void Unbind() const;

	inline uint32_t GetID() const { return m_ID; };

private:
	uint32_t m_ID;

};

#endif