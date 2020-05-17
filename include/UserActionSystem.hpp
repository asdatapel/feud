#ifndef FEUD_USERACTIONSYSTEM_HPP
#define FEUD_USERACTIONSYSTEM_HPP

#include <SFML/Window.hpp>

#include "EntityManager.hpp"
#include "UserAction.hpp"

struct UserActionSystem
{
    void update(EntityManager *entityManager, sf::Window *window)
    {
        entityManager->userActions.clear();

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // TODO(asad): this should add an event somewhere, and something else should exit
                exit(0);
            }
            else if (event.type == sf::Event::Resized)
            {
                // TODO(asad): this should add an event somewhere, and something else should call glViewport
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Q)
                {
                    // TODO(asad): this should add an event somewhere, and something else should exit
                    exit(0);
                }
                else if (event.key.code == sf::Keyboard::P)
                {
                    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else if (event.key.code == sf::Keyboard::O)
                {
                    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                else if (event.key.code == sf::Keyboard::M)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION});
                }
                else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 1});
                }
                else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 2});
                }
                else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 3});
                }
                else if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 4});
                }
                else if (event.key.code == sf::Keyboard::Num5 || event.key.code == sf::Keyboard::Numpad5)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 5});
                }
                else if (event.key.code == sf::Keyboard::Num6 || event.key.code == sf::Keyboard::Numpad6)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 6});
                }
                else if (event.key.code == sf::Keyboard::Num7 || event.key.code == sf::Keyboard::Numpad7)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 7});
                }
                else if (event.key.code == sf::Keyboard::Num8 || event.key.code == sf::Keyboard::Numpad8)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION,8});
                }
                else if (event.key.code == sf::Keyboard::Num9 || event.key.code == sf::Keyboard::Numpad9)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 9});
                }
                else if (event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Numpad0)
                {
                    entityManager->userActions.push_back({UserAction::Type::TEST_ACTION, 0});
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
    }
};

#endif //FEUD_USERACTIONSYSTEM_HPP