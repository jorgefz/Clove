#include "clovepch.h"
#include "Clove/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "stb_image.h"

namespace Clove {

	Texture2D::Texture2D(uint32_t width, uint32_t height)
		: m_width(width), m_height(height)
	{
		CLOVE_PROFILE_FUNCTION();
		m_internal_format = GL_RGBA8;
		m_data_format = GL_RGBA;

		// Initialize OpenGL texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::Texture2D(const std::string& path) 
		: m_path(path), m_renderer_id(0), m_width(0), m_height(0)
	{
		CLOVE_PROFILE_FUNCTION();
		// Retrieve image data from file
		// TO-DO: use own image loader rather than STB
		int width = 0, height = 0, channels = 0;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = nullptr;
		{
			CLOVE_PROFILE_SCOPE("stbi_load - Texture2D::Texture2D(const std::string& path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		CLOVE_ASSERT(data, "Failed to load image!");
		CLOVE_INFO("Loaded %dx%d texture '%s'", width, height, path.c_str());
		m_width = static_cast<unsigned int>(width);
		m_height = static_cast<unsigned int>(height);

		GLenum internal_format, input_format;
		switch (channels) {
		case 3:
			internal_format = GL_RGB8;
			input_format = GL_RGB;
			break;
		case 4:
			internal_format = GL_RGBA8;
			input_format = GL_RGBA;
			break;
		default:
			CLOVE_ASSERT(false, "Unsupported image format (wrong number of channels)");
		}

		m_internal_format = internal_format;
		m_data_format = input_format;

		// Initialize OpenGL texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, internal_format, m_width, m_height);
		
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		Texture2D::SetData(data);
		stbi_image_free(data);
	}

	Texture2D::~Texture2D() {
		CLOVE_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_renderer_id);
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		return CreateRef<Texture2D>(width, height);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		return std::make_shared<Texture2D>(path);
	}

	void Texture2D::SetData(void* data) {
		CLOVE_PROFILE_FUNCTION();
		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Bind(unsigned int slot) const {
		CLOVE_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_renderer_id);
	}

}


/*
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
		// -----------------------  -> Internal format -----------  -> Input format ------------
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
*/
