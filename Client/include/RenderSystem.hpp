#ifndef FEUD_RENDERSYSTEM_HPP
#define FEUD_RENDERSYSTEM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "EntityManager.hpp"
#include "GraphicsManager.hpp"
#include "Text.hpp"

glm::mat4 toMat4(const Transform &transform)
{
    return glm::scale(glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation), transform.scale);
}

struct RenderSystem
{
    RenderSystem(GraphicsManager *graphicsManager) : graphicsManager(graphicsManager){};

    void init(EntityManager *entityManager)
    {
        entityManager->secondaryFramebuffer = graphicsManager->newFramebuffer(); // TODO(asad): clean this resource up

        entityManager->baseShader = "shaders/base";
        graphicsManager->newShaderResource(entityManager->baseShader);

        entityManager->shader2D = "shaders/shader2D";
        graphicsManager->newShaderResource(entityManager->shader2D);

        sf::Image screenTexture;
        screenTexture.create(1920, 1080, sf::Color::Transparent); // TODO(asad): values should be configurable somewhere
        entityManager->baseRenderTarget = Material::Texture{"base", screenTexture, graphicsManager->uploadTexture(screenTexture.getPixelsPtr(), screenTexture.getSize().x, screenTexture.getSize().y)};

        float quad[] = {
            1.0,
            1.0,
            1.0,
            1.0,

            -1.0,
            1.0,
            0.0,
            1.0,

            -1.0,
            -1.0,
            0.0,
            0.0,

            1.0,
            -1.0,
            1.0,
            0.0,

            1.0,
            1.0,
            1.0,
            1.0,

            -1.0,
            -1.0,
            0.0,
            0.0,
        };

        int meshHandle = graphicsManager->newBuffer(entityManager->baseShader);
        graphicsManager->updateBuffer(meshHandle, quad, 24 * sizeof(float));
        entityManager->screenQuad = Drawable{meshHandle, 6};
    };

    void drawSecondary(EntityManager *entityManager)
    {
        while (!entityManager->renderTextRequests.empty())
        {
            const auto &r = entityManager->renderTextRequests.front();

            graphicsManager->bindFramebuffer(entityManager->secondaryFramebuffer);
            unsigned int texColorBuffer = entityManager->materials[r.entityId].textures[r.texIndex].handle;
            graphicsManager->bindTextureToFramebuffer(entityManager->secondaryFramebuffer, texColorBuffer);
            graphicsManager->clear(r.clearColor);

            for (auto const &td : r.textDefintions)
            {
                graphicsManager->drawText(td.font, td.height, td.text, td.position, {1000, 300});
            }

            graphicsManager->resetFramebuffer();

            entityManager->renderTextRequests.pop();
        }
    };

    void draw(EntityManager *entityManager)
    {
        graphicsManager->bindFramebuffer(entityManager->secondaryFramebuffer);
        graphicsManager->bindTextureToFramebuffer(entityManager->secondaryFramebuffer, entityManager->baseRenderTarget.handle);
        graphicsManager->clear({7, 0, 145, 255});

        // TODO(asad): this matrix stuff shouldn't be here
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.5f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 pvTransform = proj * view;

        while (!entityManager->drawRequests.empty())
        {
            const auto &dr = entityManager->drawRequests.front();

            graphicsManager->bindTextureToFramebuffer(entityManager->secondaryFramebuffer, dr.target->handle);
            graphicsManager->bindShader(dr.shader);

            for (int i = 0; i < dr.material->textures.size(); ++i)
            {
                graphicsManager->bindTexture(dr.material->textures[i].handle, i);
            }

            graphicsManager->uploadUniformMatrix4fv(dr.shader, "PVM", pvTransform * toMat4(*dr.transform));
            graphicsManager->renderBuffer(dr.drawable->meshHandle, dr.drawable->bufferSize);

            entityManager->drawRequests.pop();
        }

        // draw 2d elements
        glm::mat4 ortho = glm::ortho(0., 2., 0., 2.);

        while (!entityManager->draw2DRequests.empty())
        {
            const auto &dr = entityManager->draw2DRequests.front();

            graphicsManager->bindTextureToFramebuffer(entityManager->secondaryFramebuffer, dr.target->handle);
            graphicsManager->bindShader(entityManager->shader2D);

            graphicsManager->bindTexture(dr.texture->handle, 0);

            graphicsManager->uploadUniformMatrix4fv(entityManager->shader2D, "transform", ortho * toMat4(*dr.transform));
            graphicsManager->renderBuffer(entityManager->screenQuad.meshHandle, entityManager->screenQuad.bufferSize);

            entityManager->draw2DRequests.pop();
        }

        // draw main framebuffer to screen
        graphicsManager->resetFramebuffer();
        graphicsManager->clear({0, 0, 0, 255});
        graphicsManager->bindShader(entityManager->baseShader);
        graphicsManager->bindTexture(entityManager->baseRenderTarget.handle, 0);
        graphicsManager->renderBuffer(entityManager->screenQuad.meshHandle, entityManager->screenQuad.bufferSize);
    };

    GraphicsManager *graphicsManager;
};

#endif //FEUD_RENDERSYSTEM_HPP
