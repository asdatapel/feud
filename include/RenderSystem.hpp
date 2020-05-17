#ifndef FEUD_RENDERSYSTEM_HPP
#define FEUD_RENDERSYSTEM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "EntityManager.hpp"
#include "GraphicsManager.hpp"

glm::mat4 toMat4(const Transform &transform)
{
    return glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation);
}

struct RenderSystem
{
    RenderSystem(GraphicsManager *graphicsManager)
    {
        this->graphicsManager = graphicsManager;
    };

    void draw(EntityManager *entityManager)
    {
        // TODO(asad): this shouldn't be here
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 pvTransform = proj * view;

        for (auto const &e : entityManager->drawables)
        {
            graphicsManager->bindShader(e.second.shaderHandle);
            graphicsManager->bindTexture(e.second.textureHandle);

            graphicsManager->uploadUniformMatrix4fv(e.second.shaderHandle, "PVM", pvTransform * toMat4(e.second.transform));
            graphicsManager->renderBuffer(e.second.meshHandle, e.second.mesh->m_vertCount);
        };
    };

    GraphicsManager *graphicsManager;
};

#endif //FEUD_RENDERSYSTEM_HPP
