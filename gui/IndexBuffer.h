#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	inline unsigned int GetCount() const { return m_count; }

	void bind() const;
	void unbind() const;
private:
	unsigned int m_buffer_id;
	unsigned int m_count;
};

