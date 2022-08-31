#include "VRAMPipeline.h"

VRAMPipeline::VRAMPipeline() {
    buffer_form = BufferLayout();   // ENHANCEMENT (if Needed): Individual buffer data layouts.
    buffer_form.push<float>(3);
    buffer_form.push<float>(3);
    buffer_form.push<float>(2);

}

bool VRAMPipeline::bindVArray(unsigned int id) {
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies[id]));
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

bool VRAMPipeline::createVBuffers(int count /*Add vector of sizes as 2nd param?*/) {
    bid.push_back(0);
    bool success = GL(glGenBuffers(count, &bid.back()));
    for (int b = 0; b < count; b++) {
        bindVBuffer(b);
        GL(glBufferData(GL_ARRAY_BUFFER, buffer_form.GetStride() * 1024, nullptr, GL_DYNAMIC_DRAW));
    }
    return success;
}

bool VRAMPipeline::deleteVBuffers(int count) {
    //return GL(glGenBuffers(count, &bid));               /// FIX
    return false;
}

bool VRAMPipeline::createVAO(int count, std::vector<unsigned int> bids) {
    vao.push_back(0);
    GL(glGenVertexArrays(1, &vao.back()));
    unsigned int vaoID = vao.back();
    GL(glBindVertexArray(vaoID));
    for (int b : bids)
        addVABuffer(b);
    indicies.insert(vao.back(), (unsigned)0);
    GL(glGenBuffers(1, &indicies[vaoID]));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies[vaoID]));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW));
    return true;
}

bool VRAMPipeline::updateBufferData(int, const void*) {
    return false;
}
