#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {

public:
	glm::vec3 m_Position;
	glm::vec3 m_Forward;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_WorldUp;
	float m_Pitch;
	float m_Yaw;
	float m_Sensitivity = 0.01f; //鼠标灵敏度
	float speed_Z = 0;
	float speed_Horizon = 0;
	bool camera_MoveForward = false;
	bool camera_MoveBack = false;
	bool camera_MoveLeft = false;
	bool camera_MoveRight = false;

	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

	//通过Euler Angle（欧拉角）
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp);

	~Camera();

	glm::mat4 getViewMatrix_Using_GLM_LookAtFunction();

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);
	void updateCameraPosition();

private:
	void updateCameraVectors();
};