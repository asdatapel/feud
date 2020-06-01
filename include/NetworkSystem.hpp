#ifndef FEUD_NETWORKSYSTEM_HPP
#define FEUD_NETWORKSYSTEM_HPP

#include "EntityManager.hpp"
#include "NetworkAction.hpp"

struct TestNetworkSystem
{
    
    void update(EntityManager *entityManager)
    {
        entityManager->networkActions.clear();
        for (const auto &e : entityManager->userActions)
        {
            if (e.t == UserInput::Type::TEST_ACTION)
            {
                if (e.number >= 1 && e.number <= 8)
                {
                    std::string testAnswers[8] = {
                        "RED", "BLUE", "GREEN", "ORANGE", "PURPLE", "YELLOW", "WHITE", "BLACK"};
                    entityManager->networkActions.push_back({
                        NetworkAction::Type::FLIP,
                        e.number,
                        testAnswers[e.number - 1],
                    }); 
                }
                else if (e.number == 9)
                {
                    entityManager->networkActions.push_back({NetworkAction::Type::RESET});
                }
            }
        }
    }
};

#endif // FEUD_NETWORKSYSTEM_HPP 