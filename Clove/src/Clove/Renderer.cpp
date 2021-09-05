
#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Clove {
	void Renderer::Clear() const {
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
		shader.Bind();
		va.Bind();
		ib.Bind();
		glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, 0);
	}
}