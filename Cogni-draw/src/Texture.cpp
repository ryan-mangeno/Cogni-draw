#include "Texture.h"

#include <glad/glad.h>


// @param numSpritesVert the number of sprites on a vertical column so when we do height/numSpritesVert it will give sprite size
Texture::Texture(const std::string& file_path)
	: m_ID(0), m_FilePath(file_path), m_Width(0), m_BPP(0), m_Height(0)
{

	unsigned char* img_bytes = nullptr;
	stbi_set_flip_vertically_on_load(true);
	img_bytes = stbi_load(file_path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

	if (img_bytes == nullptr)
	{
		std::cout << "Failed to create Texture, file: " << file_path;
	}

	else
	{

		//default to red
		GLenum format = GL_RED;

		if (m_BPP == 1)
			format = GL_RED;
		else if (m_BPP == 3)
			format = GL_RGB;
		else if (m_BPP == 4)
			format = GL_RGBA;

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, img_bytes);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(img_bytes);
	}

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::bind(GLuint slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

