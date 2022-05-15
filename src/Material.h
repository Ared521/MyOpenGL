#pragma once

#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material {

public:
	Shader* m_Shader;
	unsigned int m_DiffuseID;
	unsigned int m_SpecularID;
	glm::vec3 m_Ambient;
	float m_Shininess;

	Material(Shader* shader, unsigned int diffuse, unsigned int specualr, glm::vec3 ambient, float shininess);

	~Material();

};