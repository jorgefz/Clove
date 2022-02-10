#include "clovepch.h"
#include "Clove/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Clove/Renderer/VertexArray.h"

namespace Clove {

	VertexArray::VertexArray() {
		CLOVE_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_renderer_id);
	}

	VertexArray::~VertexArray() {
		CLOVE_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void VertexArray::Bind() const {
		CLOVE_PROFILE_FUNCTION();
		glBindVertexArray(m_renderer_id);
	}

	void VertexArray::Unbind() {
		CLOVE_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	Ref<VertexArray> VertexArray::Create() {
		return CreateRef<VertexArray>();
	}

	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb) {
		CLOVE_PROFILE_FUNCTION();
		
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

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib) {
		CLOVE_PROFILE_FUNCTION();
		glBindVertexArray(m_renderer_id);
		ib->Bind();
		m_ib = ib;
	}

	

}