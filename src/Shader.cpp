#include "Shader.h"
#include "Renderer.h"
#include <iostream>



Shader::Shader(const std::string& filepath) : m_FilePath((std::string&)filepath), m_RendererID(0) {
	GLCall(ShaderProgramSource source = ParseShader(filepath));//"res/shaders/Basic.shader"
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3f_WithVector3(const std::string& name, glm::vec3 vec) {
	GLCall(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	//第一个参数提供着色器程序的位置   指明要更改的uniform变量的位置  其实在下面glGetUniformLocation函数中需要指定当前是哪个shader调用，
	//但是封装好之后，在主函数调用的时候，直接是shader.SetUniform4f(); 就是当前shader了，所以自动默认为m_RendererID，无需再选择。
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end()) {
		return m_UniformlocationCache[name];
	}
	int location;
	GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));  
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
	}

	m_UniformlocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = 0, VERTEX = 1, FRAGMENT = 2
	};

	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[1].str(), ss[2].str() };
	return { ss[1].str(), ss[2].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

