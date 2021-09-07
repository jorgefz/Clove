#include "clovepch.h"
#include "Texture.h"

// this definition must stay within this cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Clove {

	Texture::Texture()
		: m_renderer_id(0), m_filepath(), m_local_buffer(nullptr),
		m_width(0), m_height(0), m_channels(0) { }

	Texture::Texture(const std::string& filepath) 
		: m_renderer_id(0), m_filepath(filepath), m_local_buffer(nullptr),
		m_width(0), m_height(0), m_channels(0) {

		// Load image from disk
		stbi_set_flip_vertically_on_load(1);
		m_local_buffer = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_channels, 4);
		if (!m_local_buffer) std::cerr << "Error: texture not loaded" << std::endl;
		glGenTextures(1, &m_renderer_id);
		glBindTexture(GL_TEXTURE_2D, m_renderer_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//                          -> Internal format ----------   -> Input format ------------
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (m_local_buffer) stbi_image_free(m_local_buffer);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_renderer_id);
	}
	void Texture::Bind(unsigned int slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_renderer_id);
	}
	void Texture::Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	int Texture::width() { return m_width; }
	int Texture::height() { return m_height; }
}
