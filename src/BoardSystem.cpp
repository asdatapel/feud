#include "BoardSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AnimationDefinition.hpp"
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

AnimationDefinition initialTransform(unsigned int pos, unsigned int id)
{
    float x = (pos % 2) * 1.01f;
    float y = (pos / 2) * -0.15f;
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
    mat = glm::rotate(mat, glm::radians(90.f), glm::vec3(1.f, 0.0f, 0.0f));
    return AnimationDefinition{id, glm::vec3(x, y, 0), glm::quat({3.14 / 2, 0, 0}), glm::vec3(x, y, 0), glm::quat({3.14 / 2, 0, 0})};
}

void BoardSystem::init(EntityManager *entityManager, GraphicsManager *graphicsManager, int shaderHandle)
{
    for (int i = 0; i < entityManager->pieces.size(); ++i)
    {
        auto &p = entityManager->pieces[i];
        p.entity_id = entityManager->entities.size();
        entityManager->entities.push_back({p.entity_id});
        entityManager->drawables[p.entity_id] = newDrawable(graphicsManager, shaderHandle);
        entityManager->animationRequests.push(initialTransform(i, p.entity_id));
    }
}

void BoardSystem::update(EntityManager *entityManager)
{
    for (const auto &e : entityManager->userActions)
    {
        if (e.t == UserAction::Type::TEST_ACTION)
        {
            if (e.number >= 1 && e.number <= 8)
            {
                auto piece = entityManager->pieces[e.number - 1];
                AnimationDefinition t = initialTransform(e.number - 1, piece.entity_id);
                t.endRotation = glm::rotate(t.endRotation, glm::radians(-180.f), glm::vec3(1.f, 0.0f, 0.0f));
                t.length = 100;
                t.interpolation = AnimationDefinition::EaseOutCubic;
                entityManager->animationRequests.push(t);
            }
        }
    }
}