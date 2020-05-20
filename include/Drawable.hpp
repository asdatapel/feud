#ifndef MINECRAFT_CLONE_DRAWABLE_H
#define MINECRAFT_CLONE_DRAWABLE_H

#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "GraphicsManager.hpp"
#include "Model.hpp"
#include "Transform.hpp"

struct Drawable
{
    std::string shader;  // TODO(asad):  should be its own class
    int meshHandle;
    std::vector<unsigned int> textureHandles;

    size_t bufferSize;

    Transform transform;
};

struct NewDrawable
{
    unsigned int entityId;

    Model model;
    std::vector<sf::Image> textures;

    std::string shader;
};

#endif //MINECRAFT_CLONE_DRAWABLE_H
