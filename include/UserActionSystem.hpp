#ifndef FEUD_USERACTIONSYSTEM_HPP
#define FEUD_USERACTIONSYSTEM_HPP

#include "EntityManager.hpp"
#include "UserAction.hpp"

struct UserActionSystem {
    void update(EntityManager *entityManager){
        entityManager->userActions.clear();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            entityManager->userActions.push_back({UserAction::Type::TEST_ACTION});
        }
    }
};

#endif //FEUD_USERACTIONSYSTEM_HPP