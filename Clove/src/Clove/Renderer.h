#pragma once
#include "clovepch.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Clove {
	class Renderer {
	public:
		void Clear() const;
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	};

}


