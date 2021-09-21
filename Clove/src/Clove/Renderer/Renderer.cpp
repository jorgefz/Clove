#include "clovepch.h"
#include "Core.h"
#include <glad/glad.h>
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

namespace Clove {
	
	void Renderer::BeginScene() {

	}

	void Renderer::EndScene() {

	}
	
	void Renderer::Submit(const std::shared_ptr<VertexArray>& va) {
		va->Bind();
		RenderCommand::DrawIndexed(va);
	}
}