#pragma once

#include "Clove/Renderer/VertexArray.h"

namespace Clove {
	
	class RenderCommand {
	public:
		static void SetClearColor(float r, float g, float b, float a = 1.0f);
		static void Clear();
		static void DrawIndexed(const std::shared_ptr<VertexArray>& va);
	};
}