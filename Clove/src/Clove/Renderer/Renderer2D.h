#pragma once

#include "Clove/Renderer/Camera.h"
#include "Clove/Renderer/Texture.h"

namespace Clove {

	struct QuadProperties {
		// spatial properties
		glm::vec3		position		= glm::vec3{0.0f};	/* xyz location */
		glm::vec2		size			= glm::vec3{1.0f,1.0f,0.0f}; /* width,height */
		float			rotation		= 0.0f;				/* right-hand rotation in radians */
		glm::vec4		color			= glm::vec4{1.0f};	/* rgba color */
		Ref<Texture2D>	texture			= nullptr;			/* texture */
		float			tiling_factor	= 1.0f;				/* shrinks texture and repeats it to fill gaps */
	};

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& cam);
		static void EndScene();
		static void Flush();

		// primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Clove::Texture2D>& texture, float tiling_factor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Clove::Texture2D>& texture, float tiling_factor = 1.0f);
	
		static void DrawQuad(const QuadProperties& props);

	private:
		static void Renderer2D::SubmitQuad(const QuadProperties& props);
	};
}