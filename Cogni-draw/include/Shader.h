#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <unordered_map>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


ShaderProgramSource get_file_contents(const std::string& filename);

class Shader
{

	public:
		Shader() = default;
		Shader(const std::string& filename);

		void set_uniform_1f(const std::string& uni_name, const float val);
		void set_uniform_1i(const std::string& uni_name, const int val);
		void set_uniform_mat4f(const std::string& uni_name, const glm::mat4& matrix);
		void set_uniform_vec3f(const std::string& uni_name, const glm::vec3& vector);
		void set_uniform_vec4f(const std::string& uni_name, const glm::vec4& vector);

		int get_uniform(const std::string& uni_name);
			
		void bind() const;
		void unbind() const;
		void compile_errors(unsigned int shader, const char* type);

		inline uint32_t get_ID() const { return m_ID; };

	private:

		uint32_t m_ID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
};


class ShaderLibrary
{
public:
	ShaderLibrary() = default;
	~ShaderLibrary();

	void add_shader(const std::string& file_name);
	
	Shader& get_shader(const std::string& shader_name);

private:
	std::unordered_map<std::string, Shader*> m_ShaderStorage;

};

#endif