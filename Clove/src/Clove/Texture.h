#pragma once
#include "clovepch.h"

#include "Core.h"
#include "Renderer.h"

namespace Clove {

	class Texture {
	private:
		unsigned int m_renderer_id;
		std::string m_filepath;
		unsigned char* m_local_buffer;
		int m_width, m_height, m_channels; // channels = bytes per pixel (RGB, RGBA, etc)
	public:
		Texture();
		Texture(const std::string& filepath);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		int width();
		int height();
	};
}
