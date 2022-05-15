#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
	m_Position = position;
	m_WorldUp = worldUp;
	m_Forward = glm::normalize(target - position);
	m_Right = glm::normalize(glm::cross(m_Forward, worldUp)); //ͨ����������Ӧ�Ĵ�ֱ����
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp) {
	m_Position = position;
	m_WorldUp = worldUp;
	m_Pitch = pitch;
	m_Yaw = yaw;
	m_Forward.x = glm::cos(m_Pitch) * glm::sin(m_Yaw);
	m_Forward.y = glm::sin(m_Pitch);
	m_Forward.z = -1 * glm::cos(m_Pitch) * glm::cos(m_Yaw); //�� -Z�᷽��

	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp)); //ͨ��������ŷ��������������
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}

Camera::~Camera() {}


void Camera::updateCameraVectors() {
	m_Forward.x = glm::cos(m_Pitch) * glm::sin(m_Yaw);
	m_Forward.y = glm::sin(m_Pitch);
	m_Forward.z = -1 * glm::cos(m_Pitch) * glm::cos(m_Yaw); //�� -Z�᷽��

	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp)); //ͨ��������ŷ��������������
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
	m_Yaw += xOffset * m_Sensitivity;
	m_Pitch += yOffset * m_Sensitivity;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped(��ת).
	if (constrainPitch) {
		if (m_Pitch > 89.0f) m_Pitch = 89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraPosition() {
	if (camera_MoveForward) {
		m_Position.z -= -m_Forward.z * speed_Z * m_Sensitivity; //��Ϊ���� Z�Ḻ����۲죬��Ҫ�� Forward�е� z�����ȡ����
		m_Position.x += m_Forward.x * speed_Z * m_Sensitivity;
		m_Position.y += m_Forward.y * speed_Z * m_Sensitivity;
		std::cout << "W" << std::endl;
	}
	else if (camera_MoveBack) {
		m_Position.z += -m_Forward.z * speed_Z * m_Sensitivity; //��Ϊ���� Z�Ḻ����۲죬��Ҫ�� Forward�е� z�����ȡ����
		m_Position.x -= m_Forward.x * speed_Z * m_Sensitivity;
		m_Position.y -= m_Forward.y * speed_Z * m_Sensitivity;
		std::cout << "S" << std::endl;
	}
	else if (camera_MoveLeft) {
		m_Position -= m_Right * speed_Horizon * m_Sensitivity;
		std::cout << "A" << std::endl;
	}
	else if (camera_MoveRight) {
		m_Position += m_Right * speed_Horizon * m_Sensitivity;
		std::cout << "D" << std::endl;
	}
}

glm::mat4 Camera::getViewMatrix_Using_GLM_LookAtFunction() {
	return glm::lookAt(m_Position, (m_Position + m_Forward), m_WorldUp);
}


