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
    return glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation);
}

struct RenderSystem
{
    RenderSystem(GraphicsManager *graphicsManager)
    {
        this->graphicsManager = graphicsManager;
    };

    void prepare(EntityManager *entityManager)
    {
        while (!entityManager->newDrawables.empty())
        {
            const auto nd = entityManager->newDrawables.front();
            entityManager->newDrawables.pop();

            // copy data to a temporary buffer
            size_t vertexSize = (3 + 3 + (nd.model.meshes[0].uvCount * 2));
            float *data = new float[nd.model.meshes[0].vertices.size() * vertexSize];
            for (int i = 0; i < nd.model.meshes[0].vertices.size(); ++i)
            {
                int index = i * vertexSize;
                data[index] = nd.model.meshes[0].vertices[i].position[0];
                data[index + 1] = nd.model.meshes[0].vertices[i].position[1];
                data[index + 2] = nd.model.meshes[0].vertices[i].position[2];

                data[index + 3] = nd.model.meshes[0].vertices[i].normal[0];
                data[index + 4] = nd.model.meshes[0].vertices[i].normal[1];
                data[index + 5] = nd.model.meshes[0].vertices[i].normal[2];

                for (int j = 0; j < nd.model.meshes[0].uvCount; ++j)
                {
                    data[index + 6 + (j * 2)] = nd.model.meshes[0].vertices[i].uvs[j][0];
                    data[index + 7 + (j * 2)] = nd.model.meshes[0].vertices[i].uvs[j][1];
                }
            }
            int meshHandle = graphicsManager->newBuffer(nd.shader);
            graphicsManager->updateBuffer(meshHandle, data, nd.model.meshes[0].vertices.size() * vertexSize);
            delete data;

            std::vector<unsigned int> textureHandles;
            for (const auto &t : nd.textures)
            {
                textureHandles.push_back(graphicsManager->uploadTexture(t.getPixelsPtr(), t.getSize().x, t.getSize().y));
            }

            entityManager->drawables[nd.entityId] = Drawable{nd.shader, meshHandle, textureHandles, nd.model.meshes[0].vertices.size()};
        }
    };

    void draw(EntityManager *entityManager)
    {

        while (!entityManager->renderTextRequests.empty())
        {
            const auto r = entityManager->renderTextRequests.front();
            entityManager->renderTextRequests.pop();

            unsigned int fbo;
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            unsigned int texColorBuffer = entityManager->drawables[r.entityId].textureHandles[r.texIndex];
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            {
                std::cout << "XXXXXXXXXXXXXXXXXS" << std::endl;
            }

            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(200 / 255.0, 200 / 255.0, 100 / 255.0, 1);

            glm::mat4 projection = glm::ortho(0.0f, 650.f, 0.0f, 300.0f);
            graphicsManager->bindShader("shaders/text");
            graphicsManager->uploadUniformMatrix4fv("shaders/text", "Projection", projection);

            unsigned int VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);
            std::string::const_iterator c;
            float x = r.position.x;
            for (c = r.text.begin(); c != r.text.end(); c++)
            {
                Font::Character ch = r.font->characters[*c];

                float xpos = x + ch.bearing.x * 1.f;
                float ypos = r.position.y - (ch.size.y - ch.bearing.y) * 1.f;

                float w = ch.size.x * 1.f;
                float h = ch.size.y * 1.f;
                // update VBO for each character
                float vertices[6][4] = {
                    {xpos, ypos + h, 0.0f, 0.0f},
                    {xpos, ypos, 0.0f, 1.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},

                    {xpos, ypos + h, 0.0f, 0.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},
                    {xpos + w, ypos + h, 1.0f, 0.0f}};
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.textureId);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.advance >> 6) * 1.f; // bitshift by 6 to get value in pixels (2^6 = 64)
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteFramebuffers(1, &fbo);
        }

        // TODO(asad): this matrix stuff shouldn't be here
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 pvTransform = proj * view;

        for (auto const &e : entityManager->drawables)
        {
            graphicsManager->bindShader(e.second.shader);

            for (int i = 0; i < e.second.textureHandles.size(); ++i)
            {
                graphicsManager->bindTexture(e.second.textureHandles[i], i);
            }

            graphicsManager->uploadUniformMatrix4fv(e.second.shader, "PVM", pvTransform * toMat4(e.second.transform));
            graphicsManager->renderBuffer(e.second.meshHandle, e.second.bufferSize);
        };
    };

    GraphicsManager *graphicsManager;
};

#endif //FEUD_RENDERSYSTEM_HPP
