#include "Model.h"

#include <glad/glad.h>


namespace ModelLoader {

	uint32_t make_texture(const std::string& path)
	{
		
		uint32_t textureID;
		glGenTextures(1, &textureID);
		int width, height, nrComponents;

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

		if (data)
		{

			//default to red
			GLenum format = GL_RED;

			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
		}

		return textureID;
	}



	Model::Model(const char* fname) : filePath(fname) 
	{
	}

	void Model::init() 
	{
		load_model(filePath);
	}

	void Model::render(Shader* shader) 
	{
		for (int i = 0; i < meshes.size(); i++) 
		{
			meshes[i].render(shader);
		}
	}

	void Model::load_model(std::string path) {


		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_TransformUVCoords);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			std::cout << "Assimp Error: " << importer.GetErrorString() << std::endl;
			return;
		}
		process_node(scene->mRootNode, scene);

	}


	void Model::process_node(aiNode* node, const aiScene* scene) {

		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(process_mesh(mesh, scene));
		}

		// after weve processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			process_node(node->mChildren[i], scene);
		}

	}



	::ModelLoader::Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {

		std::vector<::ModelLoader::Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<::ModelLoader::Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
		{
			::ModelLoader::Vertex vertex;
			glm::vec3 vec(0.0f);

			vec.x = mesh->mVertices[i].x;
			vec.y = mesh->mVertices[i].y;
			vec.z = mesh->mVertices[i].z;
			vertex.Position = vec;

			if (mesh->HasNormals()) 
			{
				vec.x = mesh->mNormals[i].x;
				vec.y = mesh->mNormals[i].y;
				vec.z = mesh->mNormals[i].z;
				vertex.Normal = vec;
			}
			else 
			{
				vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
			}

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec(0.0f);
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;

			}
			else 
			{
				vertex.TexCoords = glm::vec2(0.f, 0.f);
			}

			vertices.push_back(vertex);
		}

		for (unsigned i = 0; i < mesh->mNumFaces; i++) 
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned j = 0; j < face.mNumIndices; j++) 
			{
				indices.push_back(face.mIndices[j]);
			}


		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<::ModelLoader::Texture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<::ModelLoader::Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());


		return ::ModelLoader::Mesh(vertices, indices, textures);
	}


	std::vector<::ModelLoader::Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<::ModelLoader::Texture> textures;

		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString path;
			mat->GetTexture(type, i, &path);

			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			std::cout << textures_loaded.size() << "\n";
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == path)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)

					break;
				}
			}

			if (!skip)
			{   // If texture hasn't been loaded already, load it
				::ModelLoader::Texture texture;

				texture.id = make_texture(path.C_Str());
				texture.type = typeName;
				texture.path = path;
				textures.push_back(texture);

				textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}

		return textures;
	}


}