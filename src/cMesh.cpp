#include "cMesh.h"
#include "utils.h"

int cMesh::loadObj(std::string path)
{
    _vertices.clear();
    _faces.clear();

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "Failed to open file " << path << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::string lineTrimmed = trim(line);
        // std::cout << lineTrimmed << std::endl;

        std::vector<std::string> splitted = split(lineTrimmed, " ");

        if (splitted[0] == "#") // comment
            continue;

        if (splitted[0] == "v") // vertex
        {
            glm::vec3 vertex;
            vertex.x = std::stof(splitted[1]);
            vertex.y = std::stof(splitted[2]);
            vertex.z = std::stof(splitted[3]);
            _vertices.push_back(vertex);
        }

        if (splitted[0] == "vn") // normal
        {
            glm::vec3 normal;
            normal.x = std::stof(splitted[1]);
            normal.y = std::stof(splitted[2]);
            normal.z = std::stof(splitted[3]);
            //make sure it's normalized
            normal = glm::normalize(normal);
            _normals.push_back(normal);
        }

        if (splitted[0] == "f") // face
        {
            sFace face;
            for (int i = 1; i < 4; i++)
            {
                std::vector<std::string> vertexInfo = split(splitted[i], "/");
                sVertexInfo vertex;
                vertex.vertex_index = std::stoi(vertexInfo[0]) - 1;
                vertex.uv_index = std::stoi(vertexInfo[1]) - 1;
                vertex.normal_index = std::stoi(vertexInfo[2]) - 1;
                face.vertices[i - 1] = vertex;
            }
            _faces.push_back(face);
        }
    }

    // print vertices count
    std::cout << "Loaded " << _vertices.size() << " vertices" << std::endl;
    std::cout << "Loaded " << _normals.size() << " normals" << std::endl;
    std::cout << "Loaded " << _faces.size() << " faces" << std::endl;

    return 0;
}

void cMesh::genBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // 3 bytes for vertex, 3 for normal, 2 for uv
    // 8 floats per vertex
    float vboData[_faces.size() * 3 * 8];
    for (int i = 0; i < _faces.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vboData[i * 24 + j * 8] = _vertices[_faces[i].vertices[j].vertex_index].x;
            vboData[i * 24 + j * 8 + 1] = _vertices[_faces[i].vertices[j].vertex_index].y;
            vboData[i * 24 + j * 8 + 2] = _vertices[_faces[i].vertices[j].vertex_index].z;

            vboData[i * 24 + j * 8 + 3] = _normals[_faces[i].vertices[j].normal_index].x;
            vboData[i * 24 + j * 8 + 4] = _normals[_faces[i].vertices[j].normal_index].y;
            vboData[i * 24 + j * 8 + 5] = _normals[_faces[i].vertices[j].normal_index].z;

            vboData[i * 24 + j * 8 + 6] = 0.0f;
            vboData[i * 24 + j * 8 + 7] = 0.0f;
        }
    }

    // unsigned int indices[_faces.size() * 3];

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _faces.size() * 3 * 8 * sizeof(float), vboData, GL_STATIC_DRAW);

    // Bind and set EBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, _faces.size() * 3 * sizeof(unsigned int), &_faces[0], GL_STATIC_DRAW);

    // Define vertex attributes
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind VAO (optional)
    glBindVertexArray(0);
}
