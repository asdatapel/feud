#ifndef MINECRAFT_CLONE_DRAWABLE_H
#define MINECRAFT_CLONE_DRAWABLE_H

#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "GraphicsManager.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

struct Drawable
{
    Mesh *mesh;
    sf::Image texture;

    int shaderHandle;
    int meshHandle;
    unsigned int textureHandle;

    Transform transform;
};

#endif //MINECRAFT_CLONE_DRAWABLE_H
