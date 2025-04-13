#ifndef UTIL_H
#define UTIL_H


#include <glm/glm.hpp>
#include <memory>

	
struct Vertex2D
{
	glm::vec2 pos;
	glm::vec4 color;
};

struct Vertex3D
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex_pos;
};


enum class Focus
{
	NONE = 0,
	DRAW,
	MODEL
};

enum MovementDir
{
	NONE = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	FORWARD,
	BACK
};


template<typename T, typename... Args>
std::shared_ptr<T> CreateRef(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;


constexpr uint32_t SCREEN_WIDTH = 1920;
constexpr uint32_t SCREEN_HEIGHT = 1080;


#endif