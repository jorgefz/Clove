#pragma once
#include "Clove/Renderer/Buffer.h"


/*

An OpenGL Vertex Array hols a vertex buffer, which is the raw vertex data,
a buffer layout, which tells the shader how to interpret this data,
and an index buffer, which describes the order in which to render the vertices.

*/

namespace Clove {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		static void Unbind();

		static Ref<VertexArray> Create();
		void AddVertexBuffer(const Ref<VertexBuffer>& vb);
		void SetIndexBuffer(const Ref<IndexBuffer>& ib);

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_vb_list; }
		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_ib; }

	private:
		std::vector<Ref<VertexBuffer>> m_vb_list;
		Ref<IndexBuffer> m_ib;
		unsigned int m_renderer_id;
	};
	

}

