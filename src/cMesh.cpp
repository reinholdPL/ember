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

    unsigned int verticeIndexes[_faces.size() * 3];
    for (int i = 0; i < _faces.size(); i++)
    {
        verticeIndexes[i * 3] = _faces[i].vertices[0].vertex_index;
        verticeIndexes[i * 3 + 1] = _faces[i].vertices[1].vertex_index;
        verticeIndexes[i * 3 + 2] = _faces[i].vertices[2].vertex_index;
    }

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _faces.size() * 3 * sizeof(float), verticeIndexes, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); // Positions
    glEnableVertexAttribArray(0);

    // Unbind VAO (optional)
    glBindVertexArray(0);
}
