#pragma once
#include <GL/glew.h>
#include <unordered_map>

#include "utils_debug.h"
#include "BufferLayout.h"

class VRAMPipeline {
public:
	VRAMPipeline();

	bool createVBuffers(int);
	bool deleteVBuffers(int);

	bool createVAO(int, std::vector<unsigned int>);

	bool updateBufferData(int, const void*);

	bool existsBuffer(unsigned int id) { return glIsBuffer(id); };
	bool existsVArray(unsigned int id) { return glIsVertexArray(id); };

	bool bindVBuffer(unsigned int id) { return GL(glBindBuffer(GL_ARRAY_BUFFER, id)); };
	bool bindVArray(unsigned int id);
	bool unbindVBuffer() { return GL(glBindBuffer(GL_ARRAY_BUFFER, 0)); };
	bool unbindVArray() { return GL(glBindVertexArray(0)); };


	~VRAMPipeline();

private:

	std::vector<unsigned int> vao;
	std::vector<unsigned int> bid;
	std::unordered_map<unsigned int, unsigned int> indicies;

	void addVABuffer(int);

	BufferLayout buffer_form;

};

