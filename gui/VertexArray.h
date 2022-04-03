#pragma once
#include "VertexBuffer.h"
#include "BufferLayout.h"
class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb, const BufferLayout &layout);

	void bind() const;
	void unbind() const;

private:
	unsigned int vao;
};