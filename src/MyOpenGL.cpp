#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "stb_image/stb_image.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "Material.h"



double lastMousePostion_X;
double lastMousePostion_Y;
bool firstMouse = true;

Camera camera(glm::vec3(0, 0, 5.0f), glm::radians(15.0f), glm::radians(0.0f), glm::vec3(0, 1.0f, 0)); //ͨ������ŷ������������(��������������ͨ��������)

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.camera_MoveForward = true; //��Ϊ�ӽ������� -Z�᷽��
		camera.camera_MoveBack = false;
		camera.speed_Z = 3.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.camera_MoveBack = true; //��Ϊ�ӽ������� Z�᷽��
		camera.camera_MoveForward = false;
		camera.speed_Z = 3.0f;
	} 
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.camera_MoveLeft = true;
		camera.camera_MoveRight = false;
		camera.speed_Horizon = 3.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.camera_MoveLeft = false;
		camera.camera_MoveRight = true;
		camera.speed_Horizon = 3.0f;
	}
	else {
		camera.camera_MoveBack = false;
		camera.camera_MoveForward = false;
		camera.camera_MoveLeft = false;
		camera.camera_MoveRight = false;
		camera.speed_Horizon = 0.0f;
		camera.speed_Z = 0.0f;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastMousePostion_X = xpos;
		lastMousePostion_Y = ypos;
		firstMouse = false;
	}

	float xOffset = (float)(xpos - lastMousePostion_X);
	float yOffset = (float)(lastMousePostion_Y - ypos); //reversed since Y-coordinates go from bottom to top

	lastMousePostion_X = xpos;
	lastMousePostion_Y = ypos;

	//������������Make sure that when pitch is out of bounds, screen doesn't get flipped(��ת). Enable constrainPitch == true.
	camera.ProcessMouseMovement(xOffset, yOffset, true);
}

int main(void)
{
	{
		GLFWwindow* window;

		/* Initialize the library */
		if (!glfwInit())
			return -1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // synchronize this with our V-sync, or otherwise our master's refresh rate(��Ƶˢ����), which for me is 60.
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //���������
		glfwSetCursorPosCallback(window, mouse_callback);

		if (glewInit() != GLEW_OK) {
			std::cout << "Error!" << std::endl;
		}

		glEnable(GL_DEPTH_TEST); //Z-Buffer���� ʵ���ڵ�


		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};

		/*float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};*/
		/*float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};*/

		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		glm::vec3 cubePositions[] = {
			  glm::vec3(0.0f,  0.0f,  0.0f),
			  glm::vec3(2.0f,  5.0f, -15.0f),
			  glm::vec3(-1.5f, -2.2f, -2.5f),
			  glm::vec3(-3.8f, -2.0f, -12.3f),
			  glm::vec3(2.4f, -0.4f, -3.5f),
			  glm::vec3(-1.7f,  3.0f, -7.5f),
			  glm::vec3(1.3f, -2.0f, -2.5f),
			  glm::vec3(1.5f,  2.0f, -2.5f),
			  glm::vec3(1.5f,  0.2f, -1.5f),
			  glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		//Blend(���)����:��ǰout��ɫ����buffer������ֵ����(��ǰҪ��Ⱦ��ĳ������������ģ��͵�ǰ���õ����⵱ǰbuffer����ֵ)��
		//Cherno�н���
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));



		VertexArray va;
		VertexBuffer vb(vertices, 8 * 36 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		/*unsigned int ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);*/

		//glEnableVertexAttribArray(0);
		////��һ���ֶ�0��ʾ��������ʾ�������ԣ�λ�á�
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); 


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture("res/textures/container.png");
		texture.Bind(Texture::e_Diffuse); //�������󶨲�����ڼ�������Ԫ�� ������������һ��    Ĭ�ϲ������� 0 ��
		//shader.SetUniform1i("u_Material.s_Diffuse", 0);
		Texture texture1("res/textures/container_border.png");
		texture.Bind(Texture::e_Spectlar);

#pragma region Init Material
		Material material(&shader, texture.GetTextureID(), texture1.GetTextureID(), glm::vec3(1.0f, 1.0f, 1.0f), 64);
#pragma endregion


		/*shader.SetUniform4f("u_Color", 1.0f, 0.5f, 0.31f, 1.0f);
		shader.SetUniform4f("u_AmbientColor", 1.0f, 1.0f, 1.0f, 1.0f);*/

		GLCall(shader.UnBind());
		GLCall(vb.UnBind());
		GLCall(va.UnBind());

		Renderer renderer;


		//Camera camera(glm::vec3(0, 0, 5.0f), glm::vec3(0, 0, 0.0f), glm::vec3(0, 1.0f, 0));


		glm::mat4 trans;
		glm::mat4 modelMatrix;
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0));
		glm::mat4 viewMatrix;
		//viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -8.0f));
		//viewMatrix = camera.getViewMatrix_Using_GLM_LookAtFunction();
		glm::mat4 projectionMatrix;
		projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);




		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			/* Render here */
			//renderer.Clear();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.Bind();
			GLCall(va.Bind());

#pragma region Setting Material -> Texture
			texture.Bind(Texture::e_Diffuse);
			texture1.Bind(Texture::e_Spectlar);
#pragma endregion

			viewMatrix = camera.getViewMatrix_Using_GLM_LookAtFunction();

			for (unsigned int i = 0; i < 10; i++)
			{
				glm::mat4 modelMatrix_InLoop;
				modelMatrix_InLoop = glm::translate(modelMatrix_InLoop, cubePositions[i]);
				float angle = 20.0f * (i + 1);

				/*modelMatrix_InLoop = glm::rotate(modelMatrix_InLoop, (float)glfwGetTime() * glm::radians(angle),
					glm::vec3(0.5f, 1.0f, 0.0f));*/

#pragma region Setting MVP
				glUniformMatrix4fv(glGetUniformLocation(shader.m_RendererID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix_InLoop));
				//glUniformMatrix4fv(glGetUniformLocation(shader.m_RendererID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
				glUniformMatrix4fv(glGetUniformLocation(shader.m_RendererID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
				glUniformMatrix4fv(glGetUniformLocation(shader.m_RendererID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
#pragma endregion


#pragma region Setting Environment
				shader.SetUniform3f("u_Color", 1.0f, 1.0f, 1.0f);
				shader.SetUniform3f("u_Ambient", 0.5f, 0.5f, 0.5f);
				shader.SetUniform3f("u_LightPos", 5.0f, 5.0f, 10.0f);
				shader.SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
				shader.SetUniform3f("u_CameraPos", camera.m_Position.x, camera.m_Position.y, camera.m_Position.z);
#pragma endregion


#pragma region Setting Material
				/*û�з�װMaterial��
				shader.SetUniform3f("u_Material.s_Ambinet", 1.0f, 1.0f, 1.0f);
				//shader.SetUniform3f("u_Material.s_Diffuse", 0.1f, 0.2f, 0.5f); //���Ϲ�����ͼ��diffuse�������������(s_Diffuse)�����Բ�����Ҫ����ֵ�ˡ�
				shader.SetUniform3f("u_Material.s_Specular", 1.0f, 1.0f, 1.0f);
				shader.SetUniform1f("u_Material.s_Shininess", 64.0f);*/
				material.m_Shader->SetUniform3f_WithVector3("u_Material.s_Ambinet", material.m_Ambient);
				material.m_Shader->SetUniform1i("u_Material.s_Diffuse", Texture::e_Diffuse);
				material.m_Shader->SetUniform1i("u_Material.s_Specular", Texture::e_Spectlar);
				//material.m_Shader->SetUniform3f_WithVector3("u_Material.s_Specular", material.m_Specular);
				material.m_Shader->SetUniform1f("u_Material.s_Shininess", material.m_Shininess);
#pragma endregion




				glDrawArrays(GL_TRIANGLES, 0, 36);
			}



			//GLCall(glUniform4f(location, red, 0.3f, 0.8f, 1.0f));

			//glDrawArrays(GL_TRIANGLES, 0, 3);

			//renderer.Draw(va, ib, shader);
			//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

			camera.updateCameraPosition();
		}
	}
	glfwTerminate();
	return 0;
}