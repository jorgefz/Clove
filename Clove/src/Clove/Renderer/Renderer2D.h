#pragma once

#include "Clove/Renderer/Camera.h"
#include "Clove/Renderer/Texture.h"

namespace Clove {

	const uint32_t QuadIndices = 6; // number of indices in one quad
	const uint32_t QuadVertices = 4; // number of vertices in one quad

	struct QuadProperties {
		// spatial properties
		glm::vec3		position		= glm::vec3{0.0f};	/* xyz location */
		glm::vec2		size			= glm::vec3{1.0f,1.0f,0.0f}; /* width,height */
		float			rotation		= 0.0f;				/* clock-wise rotation in DEGREES */
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

		struct Statistics {
			uint32_t draw_calls = 0;
			uint32_t quad_count = 0;

			uint32_t GetVertexCount() { return quad_count * QuadVertices; }
			uint32_t GetIndexCount() { return quad_count * QuadIndices; }
		};
		static Statistics& GetStats();
		static void ResetStats();

	private:
		static void SubmitQuad(const QuadProperties& props);
		static void StartNewBatch();

	};
}