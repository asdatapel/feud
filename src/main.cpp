#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "Types.h"
#include "GraphicsManager.h"
#include "shaders.hpp"
#include "Camera.h"
#include "Drawable.hpp"

int main()
{

    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    settings.depthBits = 32;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    settings.stencilBits = 4;
    settings.attributeFlags = sf::ContextSettings::Attribute::Core;
    sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL", sf::Style::Default, settings);

    window.setPosition({100, 100});

    glewExperimental = GL_TRUE;
    glewInit();
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only wireframes

    glClearColor(7 / 255.0, 56 / 255.0, 145 / 255.0, 1);

    GraphicsManager graphics;

    int shader3d = graphics.newShaderResource("shaders/base");

    Drawable piece3(&graphics, shader3d);
    piece3.loadMesh("models/piece.obj");
    piece3.loadTexture("textures/piece.png");
    piece3.upload();

    Drawable piece3_2(&graphics, shader3d);
    piece3_2.loadMesh("models/piece.obj");
    piece3_2.loadTexture("textures/piece.png");
    piece3_2.upload();

    int COUNTER = 0;

    bool cameraControl = true;
    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    cameraControl = !cameraControl;
                }
                else if (event.key.code == sf::Keyboard::Q)
                {
                    running = false;
                }
                else if (event.key.code == sf::Keyboard::M)
                {
                    //world->createRandomBoxes(boxShader);
                }
                else if (event.key.code == sf::Keyboard::P)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else if (event.key.code == sf::Keyboard::O)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                else if (event.key.code == sf::Keyboard::Num0)
                {
                }
                else if (event.key.code == sf::Keyboard::Num1)
                {
                }
                else if (event.key.code == sf::Keyboard::Num2)
                {
                }
                else if (event.key.code == sf::Keyboard::Num3)
                {
                }
                else if (event.key.code == sf::Keyboard::Num4)
                {
                }
                else if (event.key.code == sf::Keyboard::Num5)
                {
                }
                else if (event.key.code == sf::Keyboard::Num6)
                {
                }
                else if (event.key.code == sf::Keyboard::Num7)
                {
                }
                else if (event.key.code == sf::Keyboard::Num8)
                {
                }
                else if (event.key.code == sf::Keyboard::Num9)
                {
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                }
                else if (event.mouseButton.button == sf::Mouse::Button::Right)
                {
                }
                else if (event.mouseButton.button == sf::Mouse::Button::Middle)
                {
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
        }

        glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        modelMat = glm::rotate(modelMat, glm::radians(COUNTER * .001f), glm::vec3(0.1f, 0.0f, 0.0f));

        glm::mat4 model2Mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0));
        model2Mat = glm::rotate(model2Mat, glm::radians(COUNTER * .1f), glm::vec3(0.4f, 0.0f, 0.0f));
        
        COUNTER += 5;

        piece3.setTransform(modelMat);
        piece3_2.setTransform(model2Mat);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

         graphics.bindShader(shader3d);
        // graphics.uploadUniformMatrix4fv(shader3d, "PVM", proj * view * modelMat);

        piece3.draw(proj * view);
        piece3_2.draw(proj * view);

        window.display();
    }
    // release resources...

    exit(0);
    return 0;
}
