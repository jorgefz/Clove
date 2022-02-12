#include "clovepch.h"
#include "Renderer2D.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Shader.h"
#include "Clove/Renderer/RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Clove {

	const uint32_t QuadIndices = 6; // number of indices in one quad
	const uint32_t QuadVertices = 4; // number of vertices in one quad

	struct QuadVertex {
		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec4 color;
		float texid;
		float tilingf;
	};

	struct Renderer2DState {
		
		const uint32_t max_quads = 1000; // quads to draw in one batch
		const uint32_t max_vertices = max_quads * QuadVertices;
		const uint32_t max_indices = max_quads * QuadIndices;
		static const uint32_t max_texture_slots = 32;
		glm::vec4 QuadVertexPositions[4];

		uint32_t quad_count = 0;
		QuadVertex* quad_vb_base = nullptr;
		QuadVertex* quad_vb_ptr = nullptr;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		std::array<Ref<Texture2D>, max_texture_slots> texture_slots;
		uint32_t texture_slot_index = 1; //  0 -> white texture

	};

	static Renderer2DState Data;



	void Renderer2D::Init() {
		CLOVE_PROFILE_FUNCTION();

		// Vertex buffer
		Data.QuadVertexBuffer = VertexBuffer::Create(Data.max_vertices * sizeof(QuadVertex));
		Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float2, "aTexCoords" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float,  "aTextureID"},
			{ ShaderDataType::Float,  "aTilingFactor"}
		});


		// Index buffer
		uint32_t* quad_indices = new uint32_t[Data.max_indices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i != Data.max_indices; i+=QuadIndices) {
			quad_indices[i + 0] = offset + 0;
			quad_indices[i + 1] = offset + 1;
			quad_indices[i + 2] = offset + 2;
			quad_indices[i + 3] = offset + 2;
			quad_indices[i + 4] = offset + 3;
			quad_indices[i + 5] = offset + 0;
			offset += QuadVertices;
		}
		Ref<IndexBuffer> quad_ib = IndexBuffer::Create(quad_indices, Data.max_indices);
		delete[] quad_indices;

		// Vertex array
		Data.quad_vb_base = new QuadVertex[Data.max_vertices];
		Data.QuadVertexArray = VertexArray::Create();
		Data.QuadVertexArray->Bind();
		Data.QuadVertexArray->SetIndexBuffer(quad_ib);
		Data.QuadVertexArray->AddVertexBuffer(Data.QuadVertexBuffer);

		// default white texture
		Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t white_pixel = 0xFFFFFFFF; // (1.0, 1.0, 1.0, 1.0)
		Data.WhiteTexture->SetData(&white_pixel);

		int samplers[Data.max_texture_slots];
		for (int i = 0; i != Data.max_texture_slots; ++i) samplers[i] = i;

		// shader
		Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		Data.TextureShader->Bind();
		Data.TextureShader->SetUniformIntArray("uTextures", samplers, Data.max_texture_slots);

		// Reset texture slots to zero
		Data.texture_slots[0] = Data.WhiteTexture;

		// Default "quad" vertex positions
		Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}

	void Renderer2D::Shutdown() {
		CLOVE_PROFILE_FUNCTION();
		if(Data.quad_vb_base) delete[] Data.quad_vb_base;
		Data.quad_vb_base = nullptr;
		Data.quad_vb_ptr = nullptr;
	}
	
	void Renderer2D::BeginScene(const Camera& cam) {
		CLOVE_PROFILE_FUNCTION();
		Data.TextureShader->Bind();
		Data.TextureShader->SetUniformMat4f("uViewProjection", cam.GetViewProjectionMatrix());
		Data.quad_vb_ptr = Data.quad_vb_base;
		Data.quad_count = 0;
		Data.texture_slot_index = 1;
	}

	void Renderer2D::EndScene() {
		CLOVE_PROFILE_FUNCTION();
		uint32_t data_size = (uint8_t*)(Data.quad_vb_ptr) - (uint8_t*)(Data.quad_vb_base);
		Data.QuadVertexBuffer->SetData(Data.quad_vb_base, data_size);
		Renderer2D::Flush();
	}

	void Renderer2D::Flush() {
		for (uint32_t i = 0; i != Data.texture_slot_index; ++i) Data.texture_slots[i]->Bind(i);
		RenderCommand::DrawIndexed(Data.QuadVertexArray, Data.quad_count * QuadIndices);
	}


	void Renderer2D::SubmitQuad(const QuadProperties& props) {
		float texture_index = 0.0f;
		if (props.texture) {
			for (uint32_t i = 1; i != Data.texture_slot_index; ++i) {
				if (*Data.texture_slots[i].get() == *props.texture.get()) {
					texture_index = (float)i;
					break;
				}
			}
			if (texture_index == 0.0f) {
				texture_index = (float)Data.texture_slot_index;
				Data.texture_slots[Data.texture_slot_index] = props.texture;
				Data.texture_slot_index++;
			}
		}

		const glm::vec3& pos = props.position;
		const glm::vec2& size = props.size;
		glm::vec3 QuadPos[4] = {
			{pos.x, pos.y, pos.z},					{pos.x + size.x, pos.y, pos.z},
			{pos.x + size.x, pos.y + size.y, pos.z}, {pos.x, pos.y + size.y, pos.z},
		};
		glm::vec2 QuadTexPos[4] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), props.position);
		transform = glm::rotate(transform, props.rotation, { 0.0f,0.0f,1.0f });
		transform = glm::scale(transform, { props.size.x, props.size.y, 1.0f });

		for (uint32_t i = 0; i != 4; ++i) {
			Data.quad_vb_ptr->position = transform * Data.QuadVertexPositions[i]; //QuadPos[i];
			Data.quad_vb_ptr->texcoord = QuadTexPos[i];
			Data.quad_vb_ptr->color = props.color;
			Data.quad_vb_ptr->texid = texture_index;
			Data.quad_vb_ptr->tilingf = props.tiling_factor;
			Data.quad_vb_ptr++;
		}

		Data.quad_count++;
	}


	// primitives
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
		Renderer2D::DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
		CLOVE_PROFILE_FUNCTION();

		QuadProperties props{};
		props.position = pos;
		props.size = size;
		props.color = color;

		Renderer2D::SubmitQuad(props);
	}


	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& tex, float tiling) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, tex, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& tex, float tiling) {
		CLOVE_PROFILE_FUNCTION();

		QuadProperties props{};
		props.position = pos;
		props.size = size;
		props.texture = tex;
		props.tiling_factor = tiling;

		Renderer2D::SubmitQuad(props);
		return;
	}


	void Renderer2D::DrawQuad(const QuadProperties& props) {
		CLOVE_PROFILE_FUNCTION();
		Renderer2D::SubmitQuad(props);
	}


}