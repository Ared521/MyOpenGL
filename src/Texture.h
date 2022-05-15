#pragma once

#include "Renderer.h"

class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	enum TextureType
	{
		e_Diffuse,
		e_Spectlar
	};

	unsigned int GetTextureID(); //�൱��ÿ��texture����һ��ID������֮��ĵ��á�

	void Bind(unsigned int slot) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};