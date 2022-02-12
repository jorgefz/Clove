#pragma once
//#include "clovepch.h"
#include "Clove/Renderer/Renderer.h"

namespace Clove {

	class Texture {
	public:
		virtual ~Texture() = default;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual void SetData(void* data) = 0;
		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		Texture2D(uint32_t width, uint32_t height);
		Texture2D(const std::string& path);
		virtual ~Texture2D();

		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		virtual unsigned int GetWidth() override { return m_width; };
		virtual unsigned int GetHeight() override { return m_height; };

		virtual void SetData(void* data) override;

		virtual void Bind(unsigned int slot = 0) const override;

		virtual bool operator==(const Texture& other) const override { return m_renderer_id == ((Texture2D&)other).m_renderer_id; }

	private:
		unsigned int m_width, m_height;
		unsigned int m_renderer_id;
		std::string m_path;
		GLenum m_data_format, m_internal_format;
	};

}
