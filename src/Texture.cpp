#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path) : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);//texture 颠倒（翻转）
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	// 加载并生成纹理
	GLCall(glGenTextures(1, &m_RendererID));
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	// 为当前绑定的纹理对象设置环绕、过滤方式
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
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); //纹理单元 同一时间只能同时激活一个。如果激活下一个，那么上一个就会失效。
	glBindTexture(GL_TEXTURE_2D, m_RendererID);  //纹理单元可以绑定多个纹理目标：GL_TEXTURE_2D、GL_TEXTURE_1D等。
}

void Texture::UnBind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
