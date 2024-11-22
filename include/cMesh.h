#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <iomanip>

typedef struct {
    unsigned int vertex_index;
    unsigned int uv_index;
    unsigned int normal_index;
} sVertexInfo;

typedef struct {
    sVertexInfo vertices[3];
} sFace;

class cMesh
{
private:
    std::vector<glm::vec3> _vertices;
    std::vector<sFace> _faces;
    unsigned int VBO, VAO, EBO;

public:
    int loadObj(std::string path);
    void genBuffers();
    unsigned int getVAO() { return VAO; }
    unsigned int getVerticesCount() { return _vertices.size(); }
    unsigned int getFacesCount() { return _faces.size(); }
};
