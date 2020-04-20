#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "Types.h"
#include "GraphicsManager.h"
#include "World.h"
#include "Chunk.h"
#include "Player.h"
#include "shaders.hpp"
#include "Perlin.h"
#include "Camera.h"
#include "Mesh.h"

std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

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
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only wireframes 

    glClearColor(7 / 255.0, 56 / 255.0, 145 / 255.0, 1);

    GraphicsManager graphics;

    // move this all to a config file (base.config)
    std::string vert = readFile("shaders/base.vs");
    std::string frag = readFile("shaders/base.fs");
    std::list<Attribute> attributes;
    Attribute a = {0, std::string("position"), 3, 8, 0};
    attributes.push_back(a);
    a = {0, std::string("incolor"), 3, 8, 3};
    attributes.push_back(a);
    a = {0, std::string("texcoord"), 2, 8, 6};
    attributes.push_back(a);
    int shader3d = graphics.newShaderResource(vert, frag, attributes);
    GLint matrixUniform = graphics.getUniform(shader3d, "PVM");

    Mesh cube;
    cube.loadFromFile("models/cube");
    int cubeBuffer = graphics.newBuffer(shader3d);
    graphics.updateBuffer(cubeBuffer, cube.vertices, cube.vertexCount);
    
    int tex = graphics.getTexture("textures/grass.png");
    graphics.bindTexture(tex);

    graphics.bindShader(shader3d);

    // start main loop
    sf::Clock timer;

    bool cameraControl = true;
    bool running = true;
    timer.restart();
    while (running)
    {
        float elapsedTime = timer.getElapsedTime().asSeconds();

        timer.restart();

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

        //Rendering
        //glm::mat4 PV = camera.getPVMat();

        glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f),
                                     glm::vec3(0.0f, 0.0f, 2.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

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
        graphics.bindTexture(tex);
        glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, glm::value_ptr(proj * view * modelMat));

        graphics.renderBuffer(cubeBuffer, 9);

        window.display();
    }
    // release resources...

    exit(0);
    return 0;
}
