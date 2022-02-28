#include "clovepch.h"
#include "Clove/Core/Core.h"
#include <glad/glad.h>
#include "Clove/Renderer/Renderer.h"
#include "Clove/Renderer/RenderCommand.h"
#include "Clove/Renderer/Renderer2D.h"

namespace Clove {
	
	Renderer::SceneData* Renderer::m_scene_data = new Renderer::SceneData;

	void Renderer::Init() {
		CLOVE_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthoCamera& camera) {
		m_scene_data->vp = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}
	
	void Renderer::Submit(
		const Ref<VertexArray>& va,
		const Ref<Shader>& shader,
		glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_view_proj", m_scene_data->vp);
		shader->SetUniformMat4f("u_transform", transform);
		RenderCommand::DrawIndexed(va);
	}

}


