#pragma once
#include "clovepch.h"

#include "Core.h"

namespace Clove {

	// --------------------------------
	class VertexBuffer {
	private:
		unsigned int m_renderer_id;
	public:
		VertexBuffer();
		VertexBuffer(const void* data, unsigned int size);
		void Bind() const;
		void Unbind() const;
		~VertexBuffer();
	};

	// --------------------------------
	class IndexBuffer {
	private:
		unsigned int m_renderer_id;
		unsigned int m_count;
	public:
		IndexBuffer();
		IndexBuffer(const unsigned int* data, unsigned int count);
		unsigned int GetCount() const;
		void Bind() const;
		void Unbind() const;
		~IndexBuffer();
	};

	// --------------------------------------
	class VertexArray {

	private:
		unsigned int m_stride, m_count, m_renderer_id;
		GLubyte m_sdims, m_tdims, m_cdims; //spatial, texture, and color dimensions
		std::vector<char> m_data; //raw vertex byte data (unpadded)

	public:
		VertexArray(unsigned int vertices = 0); // Constructor
		~VertexArray(); // Destructor
		void Bind() const;
		void Unbind() const;

		unsigned int count(); // number of vertices
		unsigned int size(); // size in bytes of vertex data
		unsigned int stride(); // Returns stride, aka memory size of a vertex.
		char* data(); // Retrieve raw vertex data

		float&  x(unsigned int vertex);
		float&  y(unsigned int vertex);
		float&  s(unsigned int vertex);
		float&  t(unsigned int vertex);
		GLubyte& r(unsigned int vertex);
		GLubyte& g(unsigned int vertex);
		GLubyte& b(unsigned int vertex);
		GLubyte& a(unsigned int vertex);

		void UpdateAttributes(VertexBuffer& vb); // Calls buffer data and vertex attrib pointer functions
		void AddVertex(float x, float y, float s = 0.0f, float t = 0.0f, GLubyte r=255, GLubyte g=255, GLubyte b=255, GLubyte a=255);
		void SetVertexPos(unsigned int i, float x, float y);
		void SetVertexTex(unsigned int i, float s, float t);
		void SetVertexColor(unsigned int i, GLubyte r, GLubyte g, GLubyte b);
		void SetVertexAlpha(unsigned int i, GLubyte a);

		void Translate(float dx, float dy); // Moves vertices by dx and dy.
		void center(float& cx, float& cy); //returns center of rectangle in screen coordinates
	};
	

}

