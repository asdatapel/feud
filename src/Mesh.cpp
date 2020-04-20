//
// Created by Asda Tapel on 1/1/17.
//

#include <iostream>

#include "Mesh.h"

void Mesh::loadFromFile(std::string filename) {
    std::ifstream file;
    file.open(filename);

    if (!file.is_open())
    {
        std::cerr << "Could not read model " << filename << ". File does not exist." << std::endl;
    }

    int count;
    file >> count;

    vertices = new float[count];
    for (int i = 0; i < count; ++i){
        file >> vertices[i];
    }

    vertexCount = count;
}