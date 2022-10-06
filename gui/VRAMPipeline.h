#pragma once
#include <GL/glew.h>
#include <unordered_map>

#include "utils_debug.h"
#include "BufferLayout.h"

struct indexStorage {
	unsigned int id;
	unsigned int count;
	unsigned int* data;
};

class VRAMPipeline {
public:
	VRAMPipeline();

	bool createVBuffers(int, int = 2048);
	bool deleteVBuffers(int);

	bool createVAO(std::vector<unsigned int>, unsigned int = 2048, unsigned int* = nullptr);

	int getVAOIndexCount(unsigned int id) { return indicies[id].count; };

	bool updateVertexBuffer(unsigned int, const void*, unsigned int count);
	bool updateIndexBuffer(unsigned int vao_id, const unsigned int* data, unsigned int size);

	bool existsBuffer(unsigned int id) { return glIsBuffer(id); };
	bool existsVArray(unsigned int id) { return glIsVertexArray(id); };

	bool bindVBuffer(unsigned int id);
	bool bindIBuffer(unsigned int id);
	bool bindVArray(unsigned int id);
	bool unbindVBuffer() { return GL(glBindBuffer(GL_ARRAY_BUFFER, 0)); };
	bool unbindIBuffer() { return GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); };
	bool unbindVArray() { return GL(glBindVertexArray(0)); };


	~VRAMPipeline();

private:

	std::vector<unsigned int> vao;
	std::vector<unsigned int> bid;
	std::unordered_map<unsigned int, int> b_size;
	std::unordered_map<unsigned int, indexStorage> indicies;

	void addVABuffer(int);

	BufferLayout buffer_form;

};

