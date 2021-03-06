#pragma once

#include "Clove/Renderer/VertexArray.h"

namespace Clove {
	
	class RenderCommand {
	public:
		static void Init();
		static void SetClearColor(float r, float g, float b, float a = 1.0f);
		static void Clear();
		static void DrawIndexed(const Ref<VertexArray>& va, uint32_t index_count = 0);
		static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	};
}