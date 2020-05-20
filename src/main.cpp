#include <iostream>
#include <fstream>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>

#include "AnimationSystem.hpp"
#include "BoardSystem.hpp"
#include "Drawable.hpp"
#include "EntityManager.hpp"
#include "GraphicsManager.hpp"
#include "Model.hpp"
#include "RenderSystem.hpp"
#include "UserInputSystem.hpp"

#include "Text.hpp"

int main()
{

    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    settings.depthBits = 32;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    settings.stencilBits = 4;
    settings.attributeFlags = sf::ContextSettings::Attribute::Core;
    sf::Window window(sf::VideoMode(1920, 1080), "OpenGL", sf::Style::Default, settings);

    window.setPosition({100, 100});

    glewExperimental = GL_TRUE;
    glewInit();
    window.setVerticalSyncEnabled(true);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only wireframes

    //glClearColor(7 / 255.0, 56 / 255.0, 50 / 255.0, 1);
    glClearColor(7 / 255.0, 56 / 255.0, 145 / 255.0, 1);

    EntityManager entityManager;

    GraphicsManager graphics;
    graphics.newShaderResource("shaders/piece");
    graphics.newShaderResource("shaders/text");

    UserInputSystem userActionSystem;
    BoardSystem boardSystem;
    boardSystem.init(&entityManager);
    AnimationSystem animationSystem;
    RenderSystem renderSystem(&graphics);

    int COUNTER = 0;

    bool running = true;
    while (running)
    {
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


        userActionSystem.update(&entityManager, &window);
        boardSystem.update(&entityManager);
        renderSystem.prepare(&entityManager);
        animationSystem.apply(&entityManager);
        renderSystem.draw(&entityManager);

        window.display();
    }

    // TODO(asad): release resources...

    exit(0);
    return 0;
}
