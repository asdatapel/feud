#ifndef MINECRAFT_CLONE_MESH_H
#define MINECRAFT_CLONE_MESH_H

#include <string>
#include <fstream>

class Mesh
{
public:
    Mesh(std::string name);

    float *m_data;
    int m_len;
    int m_vertCount;
};

#endif //MINECRAFT_CLONE_MESH_H
