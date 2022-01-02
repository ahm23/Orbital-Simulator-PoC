#include "IndexBuffer.h"
#include "utils_debug.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count): m_count(count)
{
    GL(glGenBuffers(1, &m_buffer_id));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GL(glDeleteBuffers(1, &m_buffer_id));
}

void IndexBuffer::bind() const
{
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id));
}

void IndexBuffer::unbind() const
{
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id));
}
