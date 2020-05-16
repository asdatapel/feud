#ifndef MINECRAFT_CLONE_DRAWABLE_H
#define MINECRAFT_CLONE_DRAWABLE_H

#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "GraphicsManager.hpp"
#include "Mesh.hpp"

struct Drawable
{
    Mesh *mesh;
    sf::Image texture;

    int shaderHandle;
    int meshHandle;
    unsigned int textureHandle;

    glm::mat4 transform;

    // to load piece:
    //     data.loadMesh("models/piece.obj");
    //     data.loadTexture("textures/piece.png");
    //     data.upload();
};

#endif //MINECRAFT_CLONE_DRAWABLE_H
