#include "clovepch.h"
#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"

namespace Clove {

	VertexArray::VertexArray() {
		glCreateVertexArrays(1, &m_renderer_id);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_renderer_id);
	}

	void VertexArray::Unbind() {
		glBindVertexArray(0);
	}

	VertexArray* VertexArray::Create() {
		return new VertexArray();
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) {
		
		CLOVE_ASSERT(vb->GetLayout().GetElements().size(), " Error: vertex buffer has no layout!");
		
		glBindVertexArray(m_renderer_id);
		vb->Bind();

		// setting vertex attributes
		unsigned int index = 0;
		const BufferLayout& layout = vb->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				element.GetOpenGLType(),
				element.norm ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset
			);
			index++;
		}

		m_vb_list.push_back(vb);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) {
		glBindVertexArray(m_renderer_id);
		ib->Bind();
		m_ib = ib;
	}

	

}