#pragma once
#include "Clove/Core/Core.h"

namespace Clove {

	enum class ShaderDataType : unsigned int {
		None = 0,
		Bool,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::None:   return 0;
			case ShaderDataType::Bool:   return sizeof(bool);
			case ShaderDataType::Int:    return sizeof(int);
			case ShaderDataType::Int2:   return sizeof(int) * 2;
			case ShaderDataType::Int3:   return sizeof(int) * 3;
			case ShaderDataType::Int4:   return sizeof(int) * 4;
			case ShaderDataType::Float:  return sizeof(float);
			case ShaderDataType::Float2: return sizeof(float) * 2;
			case ShaderDataType::Float3: return sizeof(float) * 3;
			case ShaderDataType::Float4: return sizeof(float) * 4;
			case ShaderDataType::Mat3:   return sizeof(float) * 3 * 3;
			case ShaderDataType::Mat4:   return sizeof(float) * 4 * 4;
			default: CLOVE_ASSERT(false, "Unknown ShaderdataType!");
		}
		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		unsigned int size, offset;
		bool norm;

		// Methods
		BufferElement(ShaderDataType in_type, const std::string& in_name, bool in_norm = false)
			: name(in_name), type(in_type), size(0), offset(0), norm(in_norm)
		{
			size = ShaderDataTypeSize(type);
		}

		unsigned int GetComponentCount() const {
			switch (type) {
				case ShaderDataType::None:   return 0;
				case ShaderDataType::Bool:   return 1;
				case ShaderDataType::Int:    return 1;
				case ShaderDataType::Int2:   return 2;
				case ShaderDataType::Int3:   return 3;
				case ShaderDataType::Int4:   return 4;
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Mat3:   return 3 * 3;
				case ShaderDataType::Mat4:   return 4 * 4;
				default: CLOVE_ASSERT(false, "Unknown ShaderdataType"); return UINT_MAX;
			}
		}

		int GetOpenGLType() const;
	};

	class BufferLayout {
	public:
		BufferLayout() {  }
		BufferLayout(const std::initializer_list<BufferElement> elements)
			: m_elements(elements)
		{
			CalculateOffsetsAndStride();
		}
		unsigned int GetStride() const { return m_stride; }
		const std::vector<BufferElement>& GetElements() const { return m_elements; }
		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		void CalculateOffsetsAndStride(){
			unsigned int offset = 0;
			for (BufferElement& elem : m_elements) {
				elem.offset = offset;
				offset += elem.size;
			}
			m_stride = offset;
		}

	private:
		std::vector<BufferElement> m_elements;
		unsigned int m_stride = 0;
	};



	class VertexBuffer {
	public:
		VertexBuffer(float* data, unsigned int size);
		~VertexBuffer();
		void SetLayout(const BufferLayout& layout);
		const BufferLayout& GetLayout();
		void Bind();
		static void Unbind();
		static Ref<VertexBuffer> Create(float* data, unsigned int size);
	private:
		unsigned int m_id;
		BufferLayout m_layout;
	};

	class IndexBuffer {
	public:
		IndexBuffer(unsigned int* data, unsigned int count);
		~IndexBuffer();
		void Bind();
		static void Unbind();
		static Ref<IndexBuffer> Create(unsigned int* data, unsigned int count);
		unsigned int GetCount() { return m_count; }
	private:
		unsigned int m_id;
		unsigned int m_count;
	};

}