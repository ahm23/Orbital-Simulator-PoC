#pragma once
#include <vector>

class SphereRender
{
public:
	SphereRender(float radius, int sectorCount, int stackCount);
    ~SphereRender() {}
    const float* getVertices() const { return vertices.data(); }
    const float* getNormals() const { return normals.data(); }
    const float* getTexCoords() const { return texCoords.data(); }
    const unsigned int* getIndices() const { return indices.data(); }

    unsigned int getBufferDataCount() const { return getVertexCount(); }    // # of vertices
    unsigned int getBufferDataSize() const { return (unsigned int)bufferData.size() * sizeof(float); }    // # of bytes
    int getBufferDataStride() const { return bufferDataStride; }   // should be 32 bytes
    const float* getBufferData() const { return bufferData.data(); }

    unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
    unsigned int getNormalCount() const { return (unsigned int)normals.size() / 3; }
    unsigned int getTexCoordCount() const { return (unsigned int)texCoords.size() / 2; }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }
    unsigned int getLineIndexCount() const { return (unsigned int)lineIndices.size(); }
    unsigned int getTriangleCount() const { return getIndexCount() / 3; }
    unsigned int getVerticiesSize() const { return (unsigned int)vertices.size() * 4; }

    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    unsigned int getNormalSize() const { return (unsigned int)normals.size() * sizeof(float); }
    unsigned int getTexCoordSize() const { return (unsigned int)texCoords.size() * sizeof(float); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    unsigned int getLineIndexSize() const { return (unsigned int)lineIndices.size() * sizeof(unsigned int); }

private:
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    //bool smooth;
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3);
    std::vector<float> bufferData;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    int bufferDataStride;

    void buildVertices();
    void buildVerticesFlat();
};


