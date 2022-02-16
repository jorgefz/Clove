#pragma once

#include "Clove/Renderer/Texture.h"

namespace Clove {

	class SubTexture2D {
	public:
		SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max);
		const Ref<Texture2D>& GetTexture() const { return m_texture; }
		const glm::vec2* GetTexCoords() const { return m_texcoords; }

		static Ref<SubTexture2D> CreateFromCoords(
			const Ref<Texture2D>& texture,
			const glm::vec2& coords,
			const glm::vec2& cellsize,
			const glm::vec2& tilesize = { 1, 1 }
		);

	private:
		Ref<Texture2D> m_texture;
		glm::vec2 m_texcoords[4];
	};


}

