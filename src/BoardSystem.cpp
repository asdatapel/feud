#include "BoardSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Animation.hpp"
#include "UserAction.hpp"

Drawable newDrawable(GraphicsManager *graphicsManager, int shaderHandle)
{
    // TODO(asad): NO NEW!!!!
    Mesh *mesh = new Mesh("models/piece.obj");
    sf::Image texture;
    if (!texture.loadFromFile("textures/piece.png"))
    {
        std::cout << "ERRRRPR READING TEXTURE" << std::endl;
    }
    texture.flipVertically();

    int meshHandle = graphicsManager->newBuffer(shaderHandle);
    graphicsManager->updateBuffer(meshHandle, mesh->m_data, mesh->m_len);

    unsigned int textureHandle = graphicsManager->uploadTexture(texture.getPixelsPtr(), texture.getSize().x, texture.getSize().y);

    return Drawable{mesh, texture, shaderHandle, meshHandle, textureHandle};
}

Animation initialTransform(unsigned int id)
{
    float x = (id % 2) * 1.01f;
    float y = (id / 2) * 0.15f;
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
    mat = glm::rotate(mat, glm::radians(90.f), glm::vec3(1.f, 0.0f, 0.0f));
    return Animation{id, mat};
}

void BoardSystem::init(EntityManager *entityManager, GraphicsManager *graphicsManager, int shaderHandle)
{
    for (int i = 0; i < entityManager->pieces.size(); ++i)
    {
        auto &p = entityManager->pieces[i];
        p.entity_id = entityManager->entities.size();
        entityManager->entities.push_back({p.entity_id});
        entityManager->drawables[p.entity_id] = newDrawable(graphicsManager, shaderHandle);
        entityManager->animations.push(initialTransform(i));
    }
}

void BoardSystem::update(EntityManager *entityManager)
{
    for (const auto &e : entityManager->userActions)
    {
        if (e.t == UserAction::Type::TEST_ACTION)
        {
            auto piece4 = entityManager->pieces[3];
            Animation t = initialTransform(3);
            t.end = glm::rotate(t.end, glm::radians(180.f), glm::vec3(1.f, 0.0f, 0.0f));
            entityManager->animations.push(t);
        }
    }
}