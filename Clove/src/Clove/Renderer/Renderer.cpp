#include "clovepch.h"
#include "Core.h"
#include <glad/glad.h>
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

namespace Clove {
	
	Renderer::SceneData* Renderer::m_scene_data = new Renderer::SceneData;

	void Renderer::BeginScene(Camera& camera) {
		m_scene_data->vp = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}
	
	void Renderer::Submit(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader) {
		shader->Bind();
		shader->SetUniformMat4f("u_view_proj", m_scene_data->vp);
		va->Bind();
		RenderCommand::DrawIndexed(va);
	}
}