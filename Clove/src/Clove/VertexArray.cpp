#include "clovepch.h"

#include "VertexArray.h"

namespace Clove {

	// VertexBuffer ------------------------------
	VertexBuffer::VertexBuffer() : m_renderer_id(0) { }
	VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
		glGenBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		#ifdef _DEBUG
		std::cout << "Clove::VertexBuffer " << size << " bytes, id " << m_renderer_id << std::endl;
		#endif
	}
	void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}
	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	VertexBuffer::~VertexBuffer(){
		glDeleteBuffers(1, &m_renderer_id);
	}

	// IndexBuffer ------------------------------
	IndexBuffer::IndexBuffer() : m_renderer_id(0), m_count(0) { }
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_count(count) {
		glGenBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
		#ifdef _DEBUG
		std::cout << "Clove::IndexBuffer " << count << " uints, id " << m_renderer_id << std::endl;
		#endif
	}
	unsigned int IndexBuffer::GetCount() const { return m_count; }
	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	}
	void IndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_renderer_id);
	}

	// VertexArray ------------------------------	
	VertexArray::VertexArray(unsigned int vertices)
		: m_sdims(2), m_tdims(2), m_cdims(4),
		m_count(0), m_renderer_id(0), m_stride(0),
		m_data() {

		m_stride = sizeof(GLfloat) * (m_sdims + m_tdims) + sizeof(GLubyte) * m_cdims;
		glCreateVertexArrays(1, &m_renderer_id);
		for (unsigned int i = 0; i != vertices; ++i) VertexArray::AddVertex(0.0f, 0.0f);
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_renderer_id);
	}

	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	unsigned int VertexArray::count() { return m_count; }
	unsigned int VertexArray::size() { return m_data.size(); }
	unsigned int VertexArray::stride() { return m_stride; }
	char* VertexArray::data() { return m_data.data(); }

	float&  VertexArray::x(unsigned int vertex) { return *(float*) (m_data.data() + vertex * m_stride); }
	float&  VertexArray::y(unsigned int vertex) { return *(float*) (m_data.data() + vertex * m_stride + sizeof(float)); }
	float&  VertexArray::s(unsigned int vertex) { return *(float*) (m_data.data() + vertex * m_stride + 2 * sizeof(float)); }
	float&  VertexArray::t(unsigned int vertex) { return *(float*) (m_data.data() + vertex * m_stride + 3 * sizeof(float)); }
	GLubyte& VertexArray::r(unsigned int vertex) { return *(GLubyte*)(m_data.data() + vertex * m_stride + 4 * sizeof(float)); }
	GLubyte& VertexArray::g(unsigned int vertex) { return *(GLubyte*)(m_data.data() + vertex * m_stride + 4 * sizeof(float) + sizeof(GLubyte)); }
	GLubyte& VertexArray::b(unsigned int vertex) { return *(GLubyte*)(m_data.data() + vertex * m_stride + 4 * sizeof(float) + 2 * sizeof(GLubyte)); }
	GLubyte& VertexArray::a(unsigned int vertex) { return *(GLubyte*)(m_data.data() + vertex * m_stride + 4 * sizeof(float) + 3 * sizeof(GLubyte)); }

	void VertexArray::UpdateAttributes(VertexBuffer& vb) {
		unsigned int offset = 0;
		vb.Bind();
		VertexArray::Bind();

		glEnableVertexAttribArray(0); // x,y
		glVertexAttribPointer(0, m_sdims, GL_FLOAT, GL_FALSE, m_stride, (const void*)offset);
		offset += m_sdims * sizeof(float);

		glEnableVertexAttribArray(1); // s,t
		glVertexAttribPointer(1, m_tdims, GL_FLOAT, GL_FALSE, m_stride, (const void*)offset);
		offset += m_tdims * sizeof(float);
		
		glEnableVertexAttribArray(2); // r,g,b,a
		glVertexAttribPointer(2, m_cdims, GL_BYTE, GL_TRUE, m_stride, (const void*)offset);
	}
	void VertexArray::AddVertex(float x, float y, float s, float t, GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		m_data.resize(m_stride * (m_count + 1));
		char* ptr = m_data.data() + m_count * m_stride;
		this->x(m_count) = x; this->y(m_count) = y;
		this->s(m_count) = s; this->t(m_count) = t;
		this->r(m_count) = r; this->g(m_count) = g; this->b(m_count) = b;
		this->a(m_count) = a;
		m_count++;
	}
	void VertexArray::SetVertexPos(unsigned int i, float x, float y) { this->x(i) = x; this->y(i) = y; }
	void VertexArray::SetVertexTex(unsigned int i, float s, float t) { this->s(i) = s; this->t(i) = t; }
	void VertexArray::SetVertexColor(unsigned int i, GLubyte r, GLubyte g, GLubyte b) { this->r(i) = r; this->g(i) = g; this->b(i) = b; }
	void VertexArray::SetVertexAlpha(unsigned int i, GLubyte a) { this->a(i) = a; }

	void VertexArray::Translate(float dx, float dy) {
		for (unsigned int i = 0; i != m_count; ++i) {
			this->x(i) += dx; this->y(i) += dy;
		}
	}
	// Returns centroid: average of coordinates on each dimension (x,y)
	void VertexArray::center(float& cx, float& cy) {
		float sum_x = 0, sum_y = 0;
		for (unsigned int i = 0; i != m_count; ++i) {
			sum_x += this->x(i); sum_y += this->y(i);
		}
		cx = sum_x / m_count;
		cy = sum_y / m_count;
	}
}