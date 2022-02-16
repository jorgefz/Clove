#include "clovepch.h"
#include "Renderer2D.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Shader.h"
#include "Clove/Renderer/RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Clove {


	struct QuadVertex {
		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec4 color;
		float texid;
		float tilingf;
	};

	struct Renderer2DState {
		
		static const uint32_t max_quads = 20000; // quads to draw in one batch
		static const uint32_t max_vertices = max_quads * QuadVertices;
		static const uint32_t max_indices = max_quads * QuadIndices;
		static const uint32_t max_texture_slots = 32;

		uint32_t quad_count = 0;
		std::vector<QuadVertex> vertex_batch;
		std::array<uint32_t, max_indices> index_batch;
		std::vector<Ref<Texture2D>> texture_slots;
		
		glm::vec4 QuadVertexPositions[4];
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		Renderer2D::Statistics stats;
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
		uint32_t offset = 0;
		uint32_t quad_indices[] = { 0,1,2, 2,3,0 };
		for (uint32_t i = 0; i != Data.max_indices; i += QuadIndices) {
			for (uint32_t j = 0; j != QuadIndices; ++j) {
				Data.index_batch[i + j] = offset + quad_indices[j];
			}
			offset += QuadVertices;
		}
		Ref<IndexBuffer> quad_ib = IndexBuffer::Create(Data.index_batch.data(), Data.max_indices);

		// Vertex array
		Data.vertex_batch.reserve(Data.max_vertices);
		Data.QuadVertexArray = VertexArray::Create();
		Data.QuadVertexArray->Bind();
		Data.QuadVertexArray->SetIndexBuffer(quad_ib);
		Data.QuadVertexArray->AddVertexBuffer(Data.QuadVertexBuffer);

		// default white texture
		Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t white_pixel = 0xFFFFFFFF; // (1.0, 1.0, 1.0, 1.0)
		Data.WhiteTexture->SetData(&white_pixel);

		// shader
		Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		Data.TextureShader->Bind();
		int samplers[Data.max_texture_slots] = { 0 };
		for (int i = 0; i != Data.max_texture_slots; ++i) samplers[i] = i;
		Data.TextureShader->SetUniformIntArray("uTextures", samplers, Data.max_texture_slots);
		//Data.texture_slots[0] = Data.WhiteTexture;

		Data.texture_slots.reserve(Data.max_texture_slots);

		// Default "quad" vertex positions
		Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}

	void Renderer2D::Shutdown() {
		CLOVE_PROFILE_FUNCTION();
	}
	
	void Renderer2D::BeginScene(const Camera& cam) {
		CLOVE_PROFILE_FUNCTION();
		Data.TextureShader->Bind();
		Data.TextureShader->SetUniformMat4f("uViewProjection", cam.GetViewProjectionMatrix());
		
		Data.vertex_batch.clear();
		Data.texture_slots.clear();
		Data.texture_slots.push_back(Data.WhiteTexture);
		Data.quad_count = 0;
		
		//Renderer2D::ResetStats();
		
	}

	void Renderer2D::EndScene() {
		CLOVE_PROFILE_FUNCTION();

		uint32_t size = Data.vertex_batch.size() * sizeof(QuadVertex);
		Data.QuadVertexBuffer->SetData(Data.vertex_batch.data(), size);

		Renderer2D::Flush();
	}

	void Renderer2D::Flush() {
		CLOVE_PROFILE_FUNCTION();
		for (uint32_t i = 0; i != Data.texture_slots.size(); ++i) Data.texture_slots[i]->Bind(i);
		RenderCommand::DrawIndexed(Data.QuadVertexArray, Data.quad_count * QuadIndices);
		Data.stats.draw_calls++;
	}

	void Renderer2D::StartNewBatch() {
		CLOVE_PROFILE_FUNCTION();

		Renderer2D::EndScene();
		Data.vertex_batch.clear();
		Data.texture_slots.clear();
		Data.texture_slots.push_back(Data.WhiteTexture);

		Data.quad_count = 0;
	}


	void Renderer2D::SubmitQuad(const QuadProperties& props) {
		CLOVE_PROFILE_FUNCTION();

		// bound checks
		if (Data.quad_count >= Data.max_quads ||
			Data.texture_slots.size() >= Data.max_texture_slots) {
			Renderer2D::StartNewBatch();
		}
			
		Ref<Texture2D> texture = nullptr;
		if (props.subtexture) texture = props.subtexture->GetTexture();
		else if (props.texture) texture = props.texture;

		uint32_t texture_index = 0;
		if (texture){
			for (uint32_t i = 1; i != Data.texture_slots.size(); ++i) {
				if(*Data.texture_slots[i].get() == *(texture.get())){
					texture_index = i;
					break;
				}
			}
			if (texture_index == 0) {
				Data.texture_slots.push_back(texture);
				texture_index = Data.texture_slots.size() - 1;
			}
		}
		
		glm::vec2 QuadTexPos[4] = {
			{0.0f, 0.0f},	{1.0f, 0.0f},
			{1.0f, 1.0f},	{0.0f, 1.0f}
		};
		
		if (props.subtexture) {
			/*
			glm::vec2 tc = { (float)props.tile_coords[0], (float)props.tile_coords[1] };
			glm::vec2 ts = { (float)props.tile_size[0], (float)props.tile_size[1] };
			float texw = (float)props.texture->GetWidth(), texh = (float)props.texture->GetHeight();
			if (ts.x == 0.0f) ts.x = texw;
			if (ts.y == 0.0f) ts.y = texh;

			float x0 = (tc.x * ts.x) / texw;
			float y0 = (tc.y * ts.y) / texh;
			float x1 = x0 + ts.x / texw;
			float y1 = y0 + ts.y / texh;

			QuadTexPos[0] = { x0,y0 };
			QuadTexPos[1] = { x1,y0 };
			QuadTexPos[2] = { x1,y1 };
			QuadTexPos[3] = { x0,y1 };
			*/
			const glm::vec2 *texcoords = props.subtexture->GetTexCoords();
			//for (int i = 0; i != 4; ++i) QuadTexPos[i] = texcoords[i];
			std::copy(texcoords, texcoords + 4, QuadTexPos);
		}

		const glm::vec3& pos = props.position;
		const glm::vec2& size = props.size;
		const float rotation = glm::radians(props.rotation);
		glm::vec3 QuadPos[4] = {
			{pos.x, pos.y, pos.z},					 {pos.x + size.x, pos.y, pos.z},
			{pos.x + size.x, pos.y + size.y, pos.z}, {pos.x, pos.y + size.y, pos.z},
		};

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), props.position);
		if (std::abs(rotation) > 1e-3) transform = glm::rotate(transform, rotation, {0.0f,0.0f,1.0f});
		transform = glm::scale(transform, { props.size.x, props.size.y, 1.0f });

		for (uint32_t i = 0; i != QuadVertices; ++i) {
			QuadVertex vertex{};
			vertex.position = transform * Data.QuadVertexPositions[i];
			vertex.texcoord = QuadTexPos[i];
			vertex.color = props.color;
			vertex.texid = static_cast<float>(texture_index);
			vertex.tilingf = props.tiling_factor;
			Data.vertex_batch.push_back(vertex);
		}

		Data.quad_count++;
		Data.stats.quad_count++;
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

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Clove::SubTexture2D>& subtex, float tiling) {
		CLOVE_PROFILE_FUNCTION();

		QuadProperties props{};
		props.position = pos;
		props.size = size;
		props.subtexture = subtex;
		props.tiling_factor = tiling;

		Renderer2D::SubmitQuad(props);
		return;
	}


	void Renderer2D::DrawQuad(const QuadProperties& props) {
		CLOVE_PROFILE_FUNCTION();
		Renderer2D::SubmitQuad(props);
	}


	Renderer2D::Statistics& Renderer2D::GetStats() {
		return Data.stats;
	}

	void Renderer2D::ResetStats() {
		Data.stats = Statistics{};
	}



}