#include "VertexBuffer.h"
#include "utils_debug.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GL(glGenBuffers(1, &buffer_id));
    GL(glBindBuffer(GL_ARRAY_BUFFER, buffer_id));
    GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GL(glDeleteBuffers(1, &buffer_id));
}

void VertexBuffer::bind() const
{
    GL(glBindBuffer(GL_ARRAY_BUFFER, buffer_id));
}

void VertexBuffer::unbind() const
{
    GL(glBindBuffer(GL_ARRAY_BUFFER, buffer_id));
}
