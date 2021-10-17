#pragma once
//#include "clovepch.h"
#include "Renderer.h"

namespace Clove {

	class Texture {
	public:
		virtual ~Texture() = default;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;

		virtual void Bind(unsigned int slot = 0) const = 0;
	};

	class Texture2D : public Texture {
	public:
		Texture2D(const std::string& path);
		virtual ~Texture2D();
		static Ref<Texture2D> Create(const std::string& path);

		virtual unsigned int GetWidth() override { return m_width; };
		virtual unsigned int GetHeight() override { return m_height; };

		virtual void Bind(unsigned int slot = 0) const override;

	private:
		unsigned int m_width, m_height;
		unsigned int m_renderer_id;
		std::string m_path;
	};


	/*
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
	*/
}
