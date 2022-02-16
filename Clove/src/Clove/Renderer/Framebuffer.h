#pragma once

#include "Clove/Core/Core.h"

namespace Clove {


	struct FramebufferSpec {
		uint32_t width, height;
		uint32_t samples = 1;
		bool swap_chain_target = false;
	};

	class Framebuffer {
	public:
		Framebuffer(const FramebufferSpec& spec);
		~Framebuffer();

		void Revalidate();
		void Bind();
		void Unbind();

		uint32_t GetColorAttachmentID() { return m_color_attachment; }
		uint32_t GetDepthAttachmentID() { return m_depth_attachment; }

		static Ref<Framebuffer> Create(const FramebufferSpec& spec);
		const FramebufferSpec& GetSpec();

	private:
		uint32_t m_rendererID;
		uint32_t m_color_attachment, m_depth_attachment;
		FramebufferSpec m_spec;
	};

}
