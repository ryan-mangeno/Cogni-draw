#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <vector>
#include <array>
#include <type_traits> .
#include <glm/vec2.hpp>


#include "util.h"



// helper trait to check if a type has a value type, if we try to template on container 
// but use something like uint32_t, it fails
template <typename T, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};



class VBO
{
public:	
	

	// creating with allocation
	VBO(uint32_t size);

	VBO();
	~VBO();


	template <typename T>
	VBO(const T& vertices, std::enable_if_t<has_value_type<T>::value, int> = 0) 
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);

		// getting element type
		using ElementType = typename T::value_type;

		glBufferData(GL_ARRAY_BUFFER, sizeof(ElementType) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void buffer_data(std::vector<Vertex2D>& vertices);
	void buffer_data(std::array<glm::vec2, 6>& vertices);

	void bind() const;
	void unbind() const;

	inline uint32_t get_ID() const { return m_ID; }

private:
	uint32_t m_ID;


};


#endif