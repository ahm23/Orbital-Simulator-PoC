#pragma once
#include <vector>
#include <GL/glew.h>

struct LayoutElement {
	unsigned int type;
	unsigned int count;
	bool normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		//err
		return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout();

	template<typename T>
	void push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count) {
		m_elements.push_back({GL_FLOAT, count, false});
		m_stride += LayoutElement::GetSizeOfType(GL_FLOAT) * count;
	}
	template<>
	void push<unsigned int>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_INT, count, false });
		m_stride += LayoutElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}
	template<>
	void push<unsigned char>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, true });
		m_stride += LayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<LayoutElement> GetElements() const& { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }


private:
	std::vector<LayoutElement> m_elements;
	unsigned int m_stride;

};

