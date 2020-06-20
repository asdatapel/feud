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
#include "GameplayInterfaceSystem.hpp"
#include "GraphicsManager.hpp"
#include "MatchStateSystem.hpp"
#include "Model.hpp"
#include "NetworkSystem.hpp"
#include "RenderSystem.hpp"
#include "TimedEventSystem.hpp"
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
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only wireframes

    //glClearColor(7 / 255.0, 56 / 255.0, 50 / 255.0, 1);
    glClearColor(7 / 255.0, 56 / 255.0, 145 / 255.0, 1);

    std::array<MatchState::Family, 2> testFamilies = {
            MatchState::Family{"family1", {{{"p1"}, {"playerA2"}, {"playerA3"}, {"playerA4"}, {}, {}}}, 1},
            MatchState::Family{"family2", {{{"playerB1"}, {"playerB2"}, {"playerB3"}, {"playerB4"}, {"playerB5"}, {}}}, 1}};

    EntityManager entityManager;
    entityManager.playerName = "p1";

    GraphicsManager graphics;
    graphics.newShaderResource("shaders/simple");
    graphics.newShaderResource("shaders/piece");
    graphics.newShaderResource("shaders/text");
    graphics.init("shaders/text");

    UserInputSystem userActionSystem;
    TestNetworkSystem networkSystem;
    networkSystem.init();
    MatchStateSystem matchStateSystem;
    matchStateSystem.init(&entityManager);
    BoardSystem boardSystem(&graphics);
    boardSystem.init(&entityManager);
    GameplayInterfaceSystem gameplayInterfaceSystem(&graphics);
    gameplayInterfaceSystem.init(&entityManager);
    TimedEventSystem timedEventSystem;
    AnimationSystem animationSystem;
    RenderSystem renderSystem(&graphics);
    renderSystem.init(&entityManager);

    int COUNTER = 0;

    bool running = true;
    while (running)
    {
        userActionSystem.update(&entityManager, &window);
        networkSystem.update(&entityManager);
        matchStateSystem.update(&entityManager);
        boardSystem.update(&entityManager);
        gameplayInterfaceSystem.update(&entityManager);
        timedEventSystem.update(&entityManager);
        animationSystem.apply(&entityManager);
        renderSystem.drawSecondary(&entityManager);
        renderSystem.draw(&entityManager);

        window.display();
    }

    // TODO(asad): release resources...

    exit(0);
    return 0;
}
