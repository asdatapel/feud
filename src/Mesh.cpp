//
// Created by Asda Tapel on 1/1/17.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include "Mesh.h"

std::vector<std::string> split(std::string s, char elem)
{
    std::vector<std::string> t;
    std::istringstream f(s);
    std::string e;
    while (std::getline(f, e, elem))
    {
        t.push_back(e);
    }

    return t;
}

Mesh::Mesh(std::string name)
{
    std::ifstream file(name);
    if (!file.is_open())
    {
        throw std::runtime_error(std::string("failed to read file: " + name));
    }

    std::vector<glm::vec3> indexedVerts;
    std::vector<glm::vec3> indexedNormals;
    std::vector<glm::vec2> indexedUVs;
    std::vector<glm::vec3> verts;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::string line;
    while (std::getline(file, line))
    {
        auto elems = split(line, ' ');
        if (elems.size() == 0)
        {
        }
        else if (elems[0] == "v")
        {
            indexedVerts.push_back({std::stof(elems[1]), std::stof(elems[2]), std::stof(elems[3])});
        }
        else if (elems[0] == "vt")
        {
            indexedUVs.push_back({std::stof(elems[1]), std::stof(elems[2])});
        }
        else if (elems[0] == "vn")
        {
            indexedNormals.push_back({std::stof(elems[1]), std::stof(elems[2]), std::stof(elems[3])});
        }
        else if (elems[0] == "f")
        {
            std::vector<int> vrefs;
            std::for_each(elems.begin() + 1, elems.end(), [&](const std::string &e) {
                auto indices = split(e, '/');
                int vertI = std::stoi(indices[0]) - 1;
                int uvI = std::stoi(indices[1]) - 1;
                int normalI = std::stoi(indices[2]) - 1;

                verts.push_back(indexedVerts[vertI]);
                uvs.push_back(indexedUVs[uvI]);
                normals.push_back(indexedNormals[normalI]);
            });
        }
    }

    int len = verts.size() * 8;
    float *data = new float[len];
    for (int i = 0; i < verts.size(); ++i)
    {
        data[i * 8] = verts[i][0];
        data[i * 8 + 1] = verts[i][1];
        data[i * 8 + 2] = verts[i][2];

        data[i * 8 + 3] = normals[i][0];
        data[i * 8 + 4] = normals[i][1];
        data[i * 8 + 5] = normals[i][2];

        data[i * 8 + 6] = uvs[i][0];
        data[i * 8 + 7] = uvs[i][1];
    }


    m_data = data;
    m_len = len;
    m_vertCount = verts.size();
}