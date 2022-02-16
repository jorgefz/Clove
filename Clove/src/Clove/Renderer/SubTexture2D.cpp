#include "clovepch.h"
#include "SubTexture2D.h"

namespace Clove {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max) 
		: m_texture(texture)
	{
		m_texcoords[0] = { min.x, min.y };
		m_texcoords[1] = { max.x, min.y };
		m_texcoords[2] = { max.x, max.y };
		m_texcoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(
		const Ref<Texture2D>& texture,
		const glm::vec2& coords,
		const glm::vec2& cellsize,
		const glm::vec2& tilesize)
	{
		
		float texw = (float)texture->GetWidth(), texh = (float)texture->GetHeight();
		glm::vec2 min = { (coords.x * cellsize.x) / texw, (coords.y * cellsize.y) / texh };
		glm::vec2 max = { ((coords.x + tilesize.x) * cellsize.x) / texw, ((coords.y + tilesize.y) * cellsize.y) / texh };

		return CreateRef<SubTexture2D>(texture, min, max);
	}
}
