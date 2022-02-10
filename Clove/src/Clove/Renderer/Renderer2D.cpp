#include "clovepch.h"
#include "Renderer2D.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Shader.h"
#include "Clove/Renderer/RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Clove {

	struct Renderer2DState {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};
	static Renderer2DState* Data = nullptr;


	void Renderer2D::Init() {
		CLOVE_PROFILE_FUNCTION();

		if (Data) {
			CLOVE_WARN(false, "Warning: Renderer2D already initialized");
			Renderer2D::Shutdown();
		}
		Data = new Renderer2DState;

		float vertices[(3 + 2) * 4] = {
			// position			  //texture
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,   1.0f, 0.0f
		};

		// vertex buffer
		Ref<VertexBuffer> vbo;
		vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->Bind();
		vbo->SetLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float2, "aTexCoords" },
			});

		// index buffer
		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> ibo;
		ibo = IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));

		// vertex array
		Data->QuadVertexArray = VertexArray::Create();
		Data->QuadVertexArray->Bind();
		Data->QuadVertexArray->SetIndexBuffer(ibo);
		Data->QuadVertexArray->AddVertexBuffer(vbo);

		Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t white_pixel = 0xFFFFFFFF; // (1.0, 1.0, 1.0, 1.0)
		Data->WhiteTexture->SetData(&white_pixel);

		// shader
		Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		Data->TextureShader->Bind();
		Data->TextureShader->SetUniform1i("uTexture", 0); // slot to sample from

	}

	void Renderer2D::Shutdown() {
		CLOVE_PROFILE_FUNCTION();
		delete Data;
		Data = nullptr;
	}
	
	void Renderer2D::BeginScene(const Camera& cam) {
		CLOVE_PROFILE_FUNCTION();
		Data->TextureShader->Bind();
		Data->TextureShader->SetUniformMat4f("uViewProjection", cam.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {
		CLOVE_PROFILE_FUNCTION();
	}

	// primitives
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
		Renderer2D::DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
		CLOVE_PROFILE_FUNCTION();
		//Data->TextureShader->Bind();
		Data->TextureShader->SetUniform4f("uColor", color);
		Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
		transform *= glm::scale(glm::mat4(1.0f), {size.x, size.y , 1.0f});
		Data->TextureShader->SetUniformMat4f("uTransform", transform);

		Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(Data->QuadVertexArray);
	}


	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture) {
		CLOVE_PROFILE_FUNCTION();
		//Data->TextureShader->Bind();

		Data->TextureShader->SetUniform4f("uColor", glm::vec4(1.0f)); // reset color
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
		transform *= glm::scale(glm::mat4(1.0f), { size.x, size.y , 1.0f });
		Data->TextureShader->SetUniformMat4f("uTransform", transform);

		Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(Data->QuadVertexArray);
	}
}