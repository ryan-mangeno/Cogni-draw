#ifndef UTIL_H
#define UTIL_H


#include <glm/glm.hpp>

	
struct Vertex2D
{
	glm::vec2 pos;
	glm::vec4 color;
	// no tex pos since this is just for painting for now
};

struct Vertex3D
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex_pos;
};


#endif