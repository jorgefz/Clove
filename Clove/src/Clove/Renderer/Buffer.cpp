#include "clovepch.h"
#include "Buffer.h"
#include "glad/glad.h"

namespace Clove {

	int BufferElement::GetOpenGLType() const {
		switch (type) {
			case ShaderDataType::None:   return GL_NONE;
			case ShaderDataType::Bool:   return GL_BOOL;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3:   return GL_FLOAT;
			case ShaderDataType::Mat4:   return GL_FLOAT;
			default: CLOVE_ASSERT(false, "Unknown ShaderdataType!");
		}
		return 0;
	}

	VertexBuffer::VertexBuffer(float* data, unsigned int size) {
		CLOVE_PROFILE_FUNCTION();
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	Ref<VertexBuffer>  VertexBuffer::Create(float* data, unsigned int size) {
		return CreateRef<VertexBuffer>(data, size);
	}
	void VertexBuffer::Bind() {
		CLOVE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}
	void VertexBuffer::Unbind() {
		CLOVE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::SetLayout(const BufferLayout& layout) {
		CLOVE_PROFILE_FUNCTION();
		m_layout = layout;
	}
	const BufferLayout& VertexBuffer::GetLayout() {
		return m_layout;
	}
	VertexBuffer::~VertexBuffer() {
		CLOVE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_id);
	}


	IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count) {
		CLOVE_PROFILE_FUNCTION();
		m_count = count;
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}
	Ref<IndexBuffer> IndexBuffer::Create(unsigned int* data, unsigned int count) {
		return CreateRef<IndexBuffer>(data, count);
	}
	void IndexBuffer::Bind() {
		CLOVE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}
	void IndexBuffer::Unbind() {
		CLOVE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	IndexBuffer::~IndexBuffer() {
		CLOVE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_id);
	}

}