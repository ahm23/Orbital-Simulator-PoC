#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vao);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
	vb.bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset);
		offset += element.count * LayoutElement::GetSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(vao);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
