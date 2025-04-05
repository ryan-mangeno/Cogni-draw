#ifndef TEXTURE_H
#define TEXTURE_H


#include <glad/glad.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <array>

#include <stb/stb_image.h>

#include "Shader.h"



class Texture
{


public:

	Texture(const std::string& file_path);
	~Texture();

	void bind(GLuint slot = 0) const;
	void unbind() const;

	inline int get_width() const { return m_Width; }
	inline int get_height() const { return m_Height; }


	inline uint32_t get_ID() const { return m_ID; };

private:

	uint32_t m_ID;
	std::string m_FilePath;
	int m_Width, m_Height, m_BPP;

};




#endif 
