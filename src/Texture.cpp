#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path) : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);//texture �ߵ�����ת��
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	// ���ز���������
	GLCall(glGenTextures(1, &m_RendererID));
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (m_LocalBuffer)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	GLCall(stbi_image_free(m_LocalBuffer));
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}

unsigned int Texture::GetTextureID() {
	return m_RendererID;
}

void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); //����Ԫ ͬһʱ��ֻ��ͬʱ����һ�������������һ������ô��һ���ͻ�ʧЧ��
	glBindTexture(GL_TEXTURE_2D, m_RendererID);  //����Ԫ���԰󶨶������Ŀ�꣺GL_TEXTURE_2D��GL_TEXTURE_1D�ȡ�
}

void Texture::UnBind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
