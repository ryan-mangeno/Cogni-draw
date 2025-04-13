#include "Shader.h"

#include <glm/gtc/type_ptr.hpp> 
#include <glad/glad.h>



Shader::Shader(const std::string& filepath) : m_UniformLocationCache()
{

	ShaderProgramSource shaderCode = get_file_contents(filepath);


	// converting the shader source strings into character arrays for opengl
	const char* vertexSource = shaderCode.VertexSource.c_str();
	const char* fragmentSource = shaderCode.FragmentSource.c_str();


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compile_errors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compile_errors(fragmentShader, "FRAGMENT");

	// creating Shader Program Object and get its id
	m_ID = glCreateProgram();

	// attaching the vert and frag shaders to program
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);

	glLinkProgram(m_ID);
	glValidateProgram(m_ID);

	compile_errors(m_ID, "PROGRAM");

	// vertex and frag shaders now can be freed since we made program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

// reads a text file and outputs a string with everything in the text file
ShaderProgramSource get_file_contents(const std::string& filename)
{
	std::ifstream in(filename.c_str(), std::ios::binary);


	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	// 1 for vert and 1 for frag
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;


	if (in)
	{


		std::string line("");


		while (getline(in, line))
		{

			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{

					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{

					type = ShaderType::FRAGMENT;

				}
			}
			
			else 
			{
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str() , ss[1].str() };
	}
	else
	{
		throw std::runtime_error("Failed to open file: " + filename);
	}
}



void Shader::bind() const
{
	glUseProgram(m_ID);
}
void Shader::unbind() const
{
	glUseProgram(0);
}


int Shader::get_uniform(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	int loc = glGetUniformLocation(m_ID, name.c_str());
	if (loc == -1)
		std::cout << "Warning: uniform: " << name << " does not exist" << std::endl; 
	else
		m_UniformLocationCache[name] = loc;

	return loc;
}



void Shader::set_uniform_mat4f(const std::string& uni_name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(get_uniform(uni_name), 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::set_uniform_vec4f(const std::string& uni_name, const glm::vec4& vector)
{
	glUniform4f(get_uniform(uni_name), vector.x, vector.y, vector.z, vector.w);

}

void Shader::set_uniform_vec3f(const std::string& uni_name, const glm::vec3& vector)
{
	glUniform3f(get_uniform(uni_name), vector.x, vector.y, vector.z);

}

void Shader::set_uniform_1f(const std::string& uni_name, const float val)
{
	glUniform1f(get_uniform(uni_name), val);
}

void Shader::set_uniform_1i(const std::string& uni_name, const int val)
{
	glUniform1i(get_uniform(uni_name), val);
}

void Shader::compile_errors(unsigned int shader, const char* type)
{

	int logLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	char* infoLog = new char[logLength];

	if (type != "PROGRAM")
	{
		int hasCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, logLength, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		int hasLinked = 0;
		glGetProgramiv(shader, GL_LINK_STATUS, &hasLinked);
		if (hasLinked == GL_FALSE)
		{
			glGetProgramInfoLog(shader, logLength, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}

	delete[] infoLog;
}

