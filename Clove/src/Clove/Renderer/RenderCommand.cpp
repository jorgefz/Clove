#include "clovepch.h"
#include "glad/glad.h"
#include "RenderCommand.h"

namespace Clove {

	void RenderCommand::Init() {
		// Adds up alpha channels
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RenderCommand::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void RenderCommand::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& va) {
		glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
