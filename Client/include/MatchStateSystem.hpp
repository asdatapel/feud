#ifndef FEUD_MATCHSTATESYSTEM_HPP
#define FEUD_MATCHSTATESYSTEM_HPP

#include "EntityManager.hpp"

struct MatchStateSystem
{
    void init(EntityManager *entityManager)
    {
        entityManager->clientEvents.push({ClientEvent::Type::JOIN});
    };

    void update(EntityManager *entityManager)
    {
        for (auto &e : entityManager->serverEvents)
        {
            if (e.type == ServerEvent::Type::MATCH_START)
            {
                entityManager->matchState.families = e.state.families;
            }
            else if (e.type == ServerEvent::Type::ROUND_START)
            {
                entityManager->matchState.round = e.state.round;
            }
            else if (e.type == ServerEvent::Type::FACEOFF_PREPARE)
            {
                entityManager->matchState.roundStage = MatchState::RoundStage::FACEOFF;
            }
            else if (e.type == ServerEvent::Type::FACEOFF_START)
            {
                entityManager->matchState.question = e.state.question;
            }
            else if (e.type == ServerEvent::Type::BUZZ)
            {
                entityManager->matchState.buzzer = e.state.buzzer;
            }
            else if (e.type == ServerEvent::Type::PASS_OR_PLAY)
            {
                entityManager->matchState.roundStage = MatchState::RoundStage::PASS_OR_PLAY;
            }
            else if (e.type == ServerEvent::Type::FLIP)
            {
                for (int i = 0; i < entityManager->pieces.size(); ++i)
                {
                    auto const &p = e.state.answers[i];
                    if (p.revealed)
                    {
                        entityManager->matchState.answers[i] = p;
                    }
                }
                entityManager->matchState.roundPoints = e.state.roundPoints;
            }
        }

        // static int count = 0;
        // if (count == 300)
        // {
        //     entityManager->clientEvents.push({ClientEvent::Type::READY, "p1"});
        // }
        // if (count == 600 && entityManager->matchState.roundStage == MatchState::RoundStage::FACEOFF)
        // {
        //     entityManager->clientEvents.push({ClientEvent::Type::READY, "p1"});
        // }
        // if (count == 1200 && entityManager->matchState.roundStage == MatchState::RoundStage::FACEOFF)
        // {
        //     entityManager->clientEvents.push({ClientEvent::Type::READY, "p1"});
        // }
        // if (count == 1500 && entityManager->matchState.roundStage == MatchState::RoundStage::FACEOFF)
        // {
        //     entityManager->clientEvents.push({ClientEvent::Type::READY, "p1"});
        // }
        // count++;
    };
};

#endif // FEUD_MATCHSTATESYSTEM_HPP