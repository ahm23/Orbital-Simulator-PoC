#include "VertexArray.h"
#include "utils_debug.h"

VertexArray::VertexArray()
{
	GL(glGenVertexArrays(1, &vao));
	GL(glBindVertexArray(vao));
}

VertexArray::~VertexArray()
{
	GL(glDeleteVertexArrays(1, &vao));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
	vb.bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GL(glEnableVertexAttribArray(i));
		GL(glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset));
		offset += element.count * LayoutElement::GetSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	GL(glBindVertexArray(vao));
}

void VertexArray::unbind() const
{
	GL(glBindVertexArray(0));
}
