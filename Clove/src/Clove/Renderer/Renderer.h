#pragma once
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Clove/Shader.h"

namespace Clove {
	class Renderer {
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& va);
	};

}
