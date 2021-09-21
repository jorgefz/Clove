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
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_vb_list; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_ib; }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_vb_list;
		std::shared_ptr<IndexBuffer> m_ib;
		unsigned int m_renderer_id;
	};
	

}

