#include "clovepch.h"
#include "Framebuffer.h"
#include <GLAD/glad.h>

namespace Clove {

	Framebuffer::Framebuffer(const FramebufferSpec& spec)
		: m_spec(spec), m_rendererID(0)
	{
		Framebuffer::Revalidate();
	}

	Framebuffer::~Framebuffer() {
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_color_attachment);
		glDeleteTextures(1, &m_depth_attachment);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height) {
		m_spec.width = width;
		m_spec.height = height;
		Framebuffer::Revalidate();
	}

	void Framebuffer::Revalidate() {

		if (m_rendererID != 0) {
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(1, &m_color_attachment);
			glDeleteTextures(1, &m_depth_attachment);
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		// creates color buffer
		glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment);
		glBindTexture(GL_TEXTURE_2D, m_color_attachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.width, m_spec.height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			m_color_attachment, 0);

		// creates depth buffer
		glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
		glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height);
		/*
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height,
			0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		*/
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
			m_depth_attachment, 0);

		CLOVE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_spec.width, m_spec.height);
	}

	void Framebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec) {
		return CreateRef<Framebuffer>(spec);
	}
	
	const FramebufferSpec& Framebuffer::GetSpec() {
		return m_spec;
	}

}