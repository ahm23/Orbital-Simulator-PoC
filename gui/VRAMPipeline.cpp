#include "VRAMPipeline.h"

VRAMPipeline::VRAMPipeline() {
    buffer_form = BufferLayout();   // ENHANCEMENT (if Needed): Individual buffer data layouts.
    buffer_form.push<float>(3);
    buffer_form.push<float>(3);
    buffer_form.push<float>(2);

}

bool VRAMPipeline::bindVBuffer(unsigned int id) {
    GL(glBindBuffer(GL_ARRAY_BUFFER, id));
    return true;
};

bool VRAMPipeline::bindIBuffer(unsigned int id) {
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    return true;
};

bool VRAMPipeline::bindVArray(unsigned int id) {
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies[id].id));
    return GL(glBindVertexArray(id));
}

VRAMPipeline::~VRAMPipeline() {
    //deleteVBuffers(sizeof(buffer_ids) / 4);
}

void VRAMPipeline::addVABuffer(int buf_id) {
    bindVBuffer(buf_id);
    const auto& elements = buffer_form.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        GL(glEnableVertexAttribArray(i));
        GL(glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, buffer_form.GetStride(), (const void*)offset));
        offset += element.count * LayoutElement::GetSizeOfType(element.type);
    }
    return;
}


bool VRAMPipeline::deleteVBuffers(int count) {
    //return GL(glGenBuffers(count, &bid));               /// FIX
    return false;
}

bool VRAMPipeline::createVAO(std::vector<unsigned int> bids, unsigned int i_count, unsigned int* indexBuffer) {
    vao.push_back(0);
    GL(glGenVertexArrays(1, &vao.back()));
    unsigned int vaoID = vao.back();
    GL(glBindVertexArray(vaoID));
    for (int b : bids)
        addVABuffer(b);

    indicies.insert(std::make_pair(vao.back(), indexStorage { (unsigned)0, i_count, indexBuffer }));
    GL(glGenBuffers(1, &indicies[vaoID].id));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies[vaoID].id));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_count * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW));
    return true;
}

bool VRAMPipeline::updateVertexBuffer(unsigned int id, const void* data, unsigned int count) {
    bindVBuffer(id);
    GL(glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_form.GetStride() * count, data ));
    unbindVBuffer();
    return true;
}

bool VRAMPipeline::updateIndexBuffer(unsigned int vao_id, const unsigned int* data, unsigned int size) {
    bindIBuffer(indicies[vao_id].id);
    GL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data));
    unbindIBuffer();
    return true;
}



// Need to return buffer id, also just make it for 1 buffer. this is stupid.
bool VRAMPipeline::createVBuffers(int b_count, int v_count) {
    bid.push_back(0);
    bool success = GL(glGenBuffers(b_count, &bid.back()));
    const unsigned int start = (bid.size() - 1 > 1) ? bid.size() - 1 : 1;   // If size 0 then there's a problem. Handle this error.
    for (unsigned int b = start; b < start + b_count; b++) {
        bindVBuffer(b);
        GL(glBufferData(GL_ARRAY_BUFFER, buffer_form.GetStride() * v_count, nullptr, GL_DYNAMIC_DRAW));
        b_size.insert(std::pair(b, v_count));
    }
    return success;
}