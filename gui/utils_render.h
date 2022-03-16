#pragma once
#include <iostream>
#include <cmath>
#include <vector>

void addVertex(std::vector<float>* vertices, float x, float y, float z)
{
    vertices->push_back(x);
    vertices->push_back(y);
    vertices->push_back(z);
}

void addNormal(std::vector<float>* normals, float x, float y, float z)
{
    normals->push_back(x);
    normals->push_back(y);
    normals->push_back(z);
}

void g_sphere(float radius, std::vector<float>* vb, std::vector<unsigned int>* ib) {
    std::vector<float> bufferData;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord
    const double PI = acos(-1);

    int sectorCount =50;                        // longitude, # of slices
    int stackCount = 50;                         // latitude, # of stacks

    float stackStep = PI / stackCount;
    float sectorStep = 2.0 * PI / sectorCount;
    float stackAngle, sectorAngle;

    int k1, k2;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cos(stackAngle);             // r * cos(u)
        z = radius * sin(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cos(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sin(sectorAngle);             // r * cos(u) * sin(v)

            addVertex(&vertices, x, y, z);
            //vertices.push_back((x/1024.0 + 1.0) / 2.0);
            //vertices.push_back((y/768.0 + 1.0) / 2.0);
            //vertices.push_back((z/1000.0 + 1.0) / 2.0);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(&normals, nx, ny, nz);
            //normals.push_back((nx / 1024 + 1.0) / 2.0);
            //normals.push_back((ny / 768.0 + 1.0) / 2.0);
            //normals.push_back((nz / 1000.0 + 1.0) / 2.0);

            // vertex tex coord between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }

    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    std::vector<float>().swap(bufferData);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        bufferData.push_back(vertices[i]);
        bufferData.push_back(vertices[i + 1]);
        bufferData.push_back(vertices[i + 2]);

        bufferData.push_back(normals[i]);
        bufferData.push_back(normals[i + 1]);
        bufferData.push_back(normals[i + 2]);

        bufferData.push_back(texCoords[j]);
        bufferData.push_back(texCoords[j + 1]);
    }
    *vb = bufferData;
    *ib = indices;
}


