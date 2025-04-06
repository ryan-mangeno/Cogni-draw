#ifndef MESH_H
#define MESH_H

#include "Shader.h"

#include <string>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace ModelLoader {

	struct Vertex 
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture 
	{
		uint32_t id;
		std::string type;
		aiString path;
	};


	class Mesh {
	public:

		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

		void render(Shader* shader);

	private:

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		uint32_t m_VAO, m_VBO, m_EBO;

		void setup_mesh();
	};


}
#endif