#pragma once
#include "Buffer.h"

namespace Clove {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		static void Unbind();

		static VertexArray* Create();
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

