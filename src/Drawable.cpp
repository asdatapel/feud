#include "Drawable.hpp"

Drawable::Drawable(GraphicsManager *_graphicsManager, int _shaderHandle)
{
    graphicsManager = _graphicsManager;
    shaderHandle = _shaderHandle;
}

void Drawable::loadMesh(std::string filename)
{
    mesh = new Mesh(filename);
}

void Drawable::loadTexture(std::string filename)
{
    if(!texture.loadFromFile(filename)){
        std::cout << "ERRRRPR READING TEXTURE" << std::endl;
    }
    
    texture.flipVertically();
}

void Drawable::upload()
{
    meshHandle = graphicsManager->newBuffer(shaderHandle);
    graphicsManager->updateBuffer(meshHandle, mesh->m_data, mesh->m_len);

    textureHandle = graphicsManager->uploadTexture(texture.getPixelsPtr(), texture.getSize().x, texture.getSize().y);
}

void Drawable::setTransform(glm::mat4 transform)
{
    m_transform = transform;
}

void Drawable::draw()
{
    graphicsManager->bindShader(shaderHandle);
    graphicsManager->bindTexture(textureHandle);

    graphicsManager->uploadUniformMatrix4fv(shaderHandle, "PVM", m_transform);
    graphicsManager->renderBuffer(meshHandle, mesh->m_vertCount);
}

void Drawable::draw(glm::mat4 pvTransform)
{
    graphicsManager->bindShader(shaderHandle);
    graphicsManager->bindTexture(textureHandle);

    graphicsManager->uploadUniformMatrix4fv(shaderHandle, "PVM", pvTransform * m_transform);
    graphicsManager->renderBuffer(meshHandle, mesh->m_vertCount);
}