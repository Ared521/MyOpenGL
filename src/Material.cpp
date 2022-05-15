#include "Material.h"

Material::Material(Shader* shader, unsigned int diffuse, unsigned int specular, glm::vec3 ambient, float shininess) : m_Shader(shader),
				m_DiffuseID(diffuse), m_SpecularID(specular), m_Ambient(ambient), m_Shininess(shininess) {

}

Material::~Material() {

}
