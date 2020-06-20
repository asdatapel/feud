#ifndef MINECRAFT_CLONE_DRAWABLE_H
#define MINECRAFT_CLONE_DRAWABLE_H

#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "Model.hpp"
#include "Transform.hpp"

struct Material
{
    struct Texture
    {
        enum struct Type
        {
            FILE,
            BUFFER,
        };
        std::string name;
        sf::Image data;
        unsigned int handle;
    };

    std::vector<Texture> textures;
};

struct Drawable
{
    int meshHandle;
    size_t bufferSize;
};

struct DrawRequest
{
    unsigned int entityId;

    Drawable *drawable;
    Transform *transform;
    std::string shader; // TODO(asad): should be its own class
    Material *material;
    Material::Texture *target;
};

struct Draw2DRequest
{
    unsigned int entityId;

    Transform *transform;
    Material::Texture *texture;
    Material::Texture *target;
};

#endif //MINECRAFT_CLONE_DRAWABLE_H
