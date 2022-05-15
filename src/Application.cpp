//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include "Renderer.h"
//#include "VertexBuffer.h"
//#include "IndexBuffer.h"
//#include "VertexArray.h"
//#include "stb_image/stb_image.h"
//#include "Shader.h"
//#include "VertexBufferLayout.h"
//#include "Texture.h"
//
////struct ShaderProgramSource
////{
////	std::string vertexSource;
////	std::string fragmentSource;
////};
////
////static ShaderProgramSource ParseShader(const std::string& filePath) {
////	std::ifstream stream(filePath);
////
////	enum class ShaderType
////	{
////		NONE = 0, VERTEX = 1, FRAGMENT = 2
////	};
////
////	std::string line;
////	std::stringstream ss[3];
////	ShaderType type = ShaderType::NONE;
////	while (getline(stream, line)) {
////		if (line.find("#shader") != std::string::npos) {
////			if (line.find("vertex") != std::string::npos) {
////				type = ShaderType::VERTEX;
////			}
////			else if (line.find("fragment") != std::string::npos) {
////				type = ShaderType::FRAGMENT;
////			}
////		}
////		else {
////			ss[(int)type] << line << '\n';
////		}
////	}
////	return { ss[1].str(), ss[2].str() };
////}
////
////static unsigned int CompileShader(unsigned int type, const std::string& source) {
////	unsigned int id = glCreateShader(type);
////	const char* src = source.c_str();
////	glShaderSource(id, 1, &src, nullptr);
////	glCompileShader(id);
////
////	int result;
////	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
////	if (result == GL_FALSE) {
////		int length;
////		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
////		char* message = (char*)_malloca(length * sizeof(char));
////		glGetShaderInfoLog(id, length, &length, message);
////		std::cout << "Failed to compile " <<
////			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
////			std::endl;
////		std::cout << message << std::endl;
////		glDeleteShader(id);
////		return 0;
////	}
////	return id;
////}
////
////static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
////	unsigned int program = glCreateProgram();
////	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
////	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
////
////	glAttachShader(program, vs);
////	glAttachShader(program, fs);
////	glLinkProgram(program);
////	glValidateProgram(program);
////
////	glDeleteShader(vs);
////	glDeleteShader(fs);
////	return program;
////}
//
//int main(void)
//{
//	{
//		GLFWwindow* window;
//
//		/* Initialize the library */
//		if (!glfwInit())
//			return -1;
//
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//		/* Create a windowed mode window and its OpenGL context */
//		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//		if (!window)
//		{
//			glfwTerminate();
//			return -1;
//		}
//
//		/* Make the window's context current */
//		glfwMakeContextCurrent(window);
//		glfwSwapInterval(1); // synchronize this with our V-sync, or otherwise our master's refresh rate(主频刷新率), which for me is 60.
//
//
//		if (glewInit() != GLEW_OK) {
//			std::cout << "Error!" << std::endl;
//		}
//
//
//		float positions[] = {
//			-0.5f, -0.5f, 0.0f, 0.0f,
//			 0.5f, -0.5f, 1.0f, 0.0f,
//			 0.5f,  0.5f, 1.0f, 1.0f,
//			-0.5f,  0.5f, 0.0f, 1.0f
//		};
//
//		unsigned int indices[] = {
//			0, 1, 2,
//			2, 3, 0
//		};
//
//		//Blend(混合)技术:当前out颜色，与buffer，做差值计算(当前要渲染在某个像素最上面的，和当前除该点以外当前buffer做差值)。
//		//Cherno有讲。
//		GLCall(glEnable(GL_BLEND));
//		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
//		
//
//		VertexArray va;
//		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
//		VertexBufferLayout layout;
//		layout.Push<float>(2);
//		layout.Push<float>(2);
//		va.AddBuffer(vb, layout);
//
//		//VertexBuffer vb(positions, 4 * 2 * sizeof(float));
//		// 
//		//将下面代码抽象成类
//		//unsigned int buffer;
//		//glGenBuffers(1, &buffer);
//		//glBindBuffer(GL_ARRAY_BUFFER, buffer);
//		//glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);//第三个参数，默认是const void* data(空类型)，可以传其他类型(const unsigned int* data)
//
//		IndexBuffer ib(indices, 6);
//
//		/*unsigned int ibo;
//		glGenBuffers(1, &ibo);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);*/
//
//		//glEnableVertexAttribArray(0);
//		////第一个字段0表示索引，表示的是属性：位置。
//		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); 
//
//
//		Shader shader("res/shaders/Basic.shader");
//		shader.Bind();
//
//		//GLCall(ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"));
//		//unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
//		//GLCall(glUseProgram(shader));
//		//GLCall(int location = glGetUniformLocation(shader, "u_Color"));
//		//ASSERT(location != -1); //if we get -1, it means that it can not find our uniform.
//		//GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
//		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
//
//
//		Texture texture("res/textures/Cherno.png");
//		texture.Bind();
//		shader.SetUniform1i("u_Texture", 0);
//
//		GLCall(shader.UnBind());
//		GLCall(vb.UnBind());
//		GLCall(va.UnBind());
//		GLCall(ib.UnBind());
//
//		Renderer renderer;
//
//
//		float red = 0.0f;
//		float increment = 0.025f;
//
//		/* Loop until the user closes the window */
//		while (!glfwWindowShouldClose(window))
//		{
//			/* Render here */
//			renderer.Clear();
//			
//
//			shader.Bind();
//			shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);
//			//GLCall(glUniform4f(location, red, 0.3f, 0.8f, 1.0f));
//
//			//glDrawArrays(GL_TRIANGLES, 0, 3);
//
//			renderer.Draw(va, ib, shader);
//			//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
//
//
//			if (red > 1.0f) {
//				increment = -0.05f;
//			}
//			else if (red < 0.0f) {
//				increment = 0.05f;
//			}
//			red += increment;
//
//			/* Swap front and back buffers */
//			glfwSwapBuffers(window);
//
//			/* Poll for and process events */
//			glfwPollEvents();
//		}
//	}
//	glfwTerminate();
//	return 0;
//}