#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb/stb_image.h>

namespace ModelLoader {

	uint32_t make_texture(const std::string& path);


	class Model 
	{
	public:
		Model(const std::string& fname);

		void init();
		void render(Shader* shader);

	private:
		void load_model(const std::string& path);
		void process_node(aiNode* node, const aiScene* scene);

		::ModelLoader::Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
		std::vector<::ModelLoader::Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string typeName);

	private:

		std::vector<::ModelLoader::Mesh> meshes;
		std::vector<::ModelLoader::Texture> textures;
		std::vector<::ModelLoader::Texture> textures_loaded;

		std::string filePath;
	};

}

#endif