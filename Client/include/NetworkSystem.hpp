#ifndef FEUD_NETWORKSYSTEM_HPP
#define FEUD_NETWORKSYSTEM_HPP

#include "EntityManager.hpp"
#include "NetworkEvent.hpp"

#include <thread>
std::queue<std::string> commands;
void inputloop()
{
    for (std::string line; std::getline(std::cin, line);)
    {
        commands.push(line);
    }
}

struct TestNetworkSystem
{
    MatchState testState;
    std::thread x;

    void init()
    {
        testState.families = {
            MatchState::Family{"family1", {{{"playerA1"}, {"playerA2"}, {"playerA3"}, {"playerA4"}, {}, {}}}, 4},
            MatchState::Family{"family2", {{{"playerB1"}, {"playerB2"}, {"playerB3"}, {"playerB4"}, {"playerB5"}, {}}}, 5}};

        x = std::thread(inputloop);
    };

    void update(EntityManager *entityManager)
    {
        entityManager->serverEvents.clear();

        for (const auto &e : entityManager->userActions)
        {
            if (e.t == UserInput::Type::TEST_ACTION)
            {
                if (e.number >= 1 && e.number <= 8)
                {
                    std::string testAnswers[8] = {
                        "RED", "BLUE", "GREEN", "ORANGE", "PURPLE", "YELLOW", "WHITE", "BLACK"};
                    int testScores[8] = {
                        10, 15, 99, 75, 28, 69, 32, 0};

                    testState.answers[e.number - 1] = {true, testAnswers[e.number - 1], testScores[e.number - 1]};
                    testState.roundPoints += testScores[e.number - 1];

                    MatchState sparseState;
                    sparseState.answers[e.number - 1] = {true, testAnswers[e.number - 1], testScores[e.number - 1]};
                    sparseState.roundPoints = testState.roundPoints;

                    entityManager->serverEvents.push_back({
                        ServerEvent::Type::FLIP,
                        sparseState,
                    });
                }
                else if (e.number == 9)
                {
                    entityManager->serverEvents.push_back({ServerEvent::Type::ROUND_END});
                }
            }
        }

        while (!commands.empty())
        {
            std::string command = commands.front();
            commands.pop();

            if (command == "incorrect")
            {
                testState.incorrects += 1;

                MatchState sparseState;
                sparseState.incorrects = testState.incorrects;

                entityManager->serverEvents.push_back({ServerEvent::Type::INCORRECT, sparseState});
            }
            else if (command == "new_round")
            {
                MatchState sparseState;
                sparseState.round = testState.round;

                entityManager->serverEvents.push_back({ServerEvent::Type::ROUND_START, sparseState});
            }
            else if (command == "end_round")
            {
                testState.round += 1;

                MatchState sparseState;
                sparseState.round = testState.round;

                entityManager->serverEvents.push_back({ServerEvent::Type::ROUND_END, sparseState});
            }
            else if (command == "start_faceoff")
            {
                testState.question = "What is round " + std::to_string(testState.round) + "?";

                MatchState sparseState;
                sparseState.question = testState.question;

                entityManager->serverEvents.push_back({ServerEvent::Type::FACEOFF_START, sparseState});
            }
        }

        // while (!entityManager->clientEvents.empty())
        // {
        //     ClientEvent event = entityManager->clientEvents.front();
        //     entityManager->clientEvents.pop();

        //     if (event.type == ClientEvent::Type::BUZZ)
        //     {
        //         //TODO(asad): if we're in a buzzable state(FACEOFF and noone has buzzed), and the player is in the faceoff
        //         testState.roundStage = MatchState::RoundStage::PASS_OR_PLAY;
        //         testState.buzzer = 1;

        //         MatchState sparseState;
        //         sparseState.roundStage = testState.roundStage;
        //         sparseState.buzzer = testState.buzzer;

        //         //TODO(asad): THIS SHOULD NOT BE SENT HERE
        //         sparseState.families = testState.families;

        //         entityManager->serverEvents.push_back({ServerEvent::Type::BUZZ, sparseState});
        //     }
        // }

        entityManager->matchState = testState;
    }
};

#endif // FEUD_NETWORKSYSTEM_HPP