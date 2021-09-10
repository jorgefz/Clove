#pragma once
#include "clovepch.h"

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
		uint8_t m_sdims, m_tdims, m_cdims; //spatial, texture, and color dimensions
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
		uint8_t& r(unsigned int vertex);
		uint8_t& g(unsigned int vertex);
		uint8_t& b(unsigned int vertex);
		uint8_t& a(unsigned int vertex);

		void UpdateAttributes(VertexBuffer& vb); // Calls buffer data and vertex attrib pointer functions
		void AddVertex(float x, float y, float s = 0.0f, float t = 0.0f, uint8_t r=255, uint8_t g=255, uint8_t b=255, uint8_t a=255);
		void SetVertexPos(unsigned int i, float x, float y);
		void SetVertexTex(unsigned int i, float s, float t);
		void SetVertexColor(unsigned int i, uint8_t r, uint8_t g, uint8_t b);
		void SetVertexAlpha(unsigned int i, uint8_t a);

		void Translate(float dx, float dy); // Moves vertices by dx and dy.
		void center(float& cx, float& cy); //returns center of rectangle in screen coordinates
	};
	

}

