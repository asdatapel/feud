#ifndef MINECRAFT_CLONE_DRAWABLE_H
#define MINECRAFT_CLONE_DRAWABLE_H

#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "GraphicsManager.h"
#include "Mesh.h"

class Drawable
{
public:
    Drawable(GraphicsManager *_graphicsManager, int _shaderHandle);
    void loadMesh(std::string filename);
    void loadTexture(std::string filename);
    void upload();

    void setTransform(glm::mat4 tranform);

    void draw();
    void draw(glm::mat4 pvTransform);

private:
    GraphicsManager *graphicsManager;

    Mesh *mesh;
    sf::Image texture;

    int shaderHandle;
    int meshHandle;
    unsigned int textureHandle;

    glm::mat4 m_transform;
};

#endif //MINECRAFT_CLONE_DRAWABLE_H
