#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &buffer_id);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}
