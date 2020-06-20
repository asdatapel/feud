#ifndef FEUD_SERVERSYSTEM_HPP
#define FEUD_SERVERSYSTEM_HPP

#include "EntityManager.hpp"

struct ServerEntityManager
{
    // pairs of ServerEvents and destinations
    std::queue<std::pair<ServerEvent, std::string>> pendingServerEvents;
    std::queue<std::pair<ClientEvent, std::string>> clientEvents;
};

struct ServerSystem
{
    MatchState matchState;

    // TODO(asad): client state that should be in a struct probably
    std::unordered_map<std::string, bool> clientStates;
    size_t readyCount;
    bool waiting;
    std::function<void(ServerEntityManager *, EntityManager *)> nextStage;

    void init(std::array<MatchState::Family, 2> families)
    {
        // set up MatchState
        matchState = {};

        matchState.families = families;

        matchState.round = 0;
        matchState.roundStage = MatchState::RoundStage::START;
        matchState.roundPoints = 0;

        matchState.playingFamily = 0;
        matchState.currentPlayers = {0, 0};
        matchState.buzzer = -1;
        matchState.faceoffWinner = -1;

        matchState.question = "";
        matchState.answers = {};
        matchState.scores = {};
        matchState.incorrects = 0;

        // init client state
        auto initClientState = [&](const MatchState::Player &p) {
            assert(clientStates.count(p.name) == 0); // error: player is repeated
            clientStates[p.name] = false;
        };
        for (int i = 0; i < families[0].playerCount; ++i)
        {
            initClientState(families[0].players[i]);
        }
        for (int i = 0; i < families[1].playerCount; ++i)
        {
            initClientState(families[1].players[i]);
        }

        readyCount = 0;

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->startRound(se, e);
        };
        waiting = true;
    };

    void tick(ServerEntityManager *serverEntityManager, EntityManager *entityManager)
    {
        while (!entityManager->clientEvents.empty())
        {
            ClientEvent event = entityManager->clientEvents.front();
            entityManager->clientEvents.pop();

            if (clientStates.count(event.source) == 0)
            {
                // error, player sending message isn't in game
                return;
            }

            // a join can happen anytime (??)
            if (event.type == ClientEvent::Type::JOIN)
            {
                handleJOIN(serverEntityManager, entityManager, event);
            }

            if (waiting)
            {
                if (event.type == ClientEvent::Type::READY)
                {
                    handleREADY(serverEntityManager, entityManager, event);
                }
            }
            else
            {
                if (event.type == ClientEvent::Type::BUZZ)
                {
                    handleBUZZ(serverEntityManager, entityManager, event);
                }
                else if (event.type == ClientEvent::Type::PASS)
                {
                    handlePASS(serverEntityManager, entityManager, event);
                }
                else if (event.type == ClientEvent::Type::PLAY)
                {
                    handlePLAY(serverEntityManager, entityManager, event);
                }
                else if (event.type == ClientEvent::Type::ANSWER)
                {
                    handleANSWER(serverEntityManager, entityManager, event);
                }
            }
        }

        entityManager->matchState = matchState;
    };

    void handleJOIN(ServerEntityManager *serverEntityManager, EntityManager *entityManager, ClientEvent msg)
    {
        MatchState sparseState;
        sparseState.families = matchState.families;
        entityManager->serverEvents.push_back({ServerEvent::Type::MATCH_START, sparseState}); // TODO(asad): direct this to the correct client
    };

    void handleREADY(ServerEntityManager *serverEntityManager, EntityManager *entityManager, ClientEvent msg)
    {
        if (!clientStates[msg.source])
        {
            readyCount++;
            clientStates[msg.source] = true;
        }
        if (readyCount > 0 /*clientStates.size()*/)
        {
            // proceed to next stage of match
            waiting = false;
            readyCount = 0;
            for (auto &s : clientStates)
            {
                s.second = false;
            }

            nextStage(serverEntityManager, entityManager);
        }
    };

    void handleBUZZ(ServerEntityManager *serverEntityManager, EntityManager *entityManager, ClientEvent msg)
    {
        if (matchState.roundStage != MatchState::RoundStage::FACEOFF || matchState.buzzer != -1)
        {
            return;
        }

        int buzzer = -1;
        const auto &[faceoffer0, faceoffer1] = matchState.whoIsInFaceoffNames();
        if (msg.source == faceoffer0)
        {
            buzzer = 0;
        }
        else if (msg.source == faceoffer1)
        {
            buzzer = 1;
        }
        else
        {
            return;
        }

        matchState.buzzer = buzzer;

        MatchState sparseState;
        sparseState.buzzer = matchState.buzzer;
        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::BUZZ, sparseState});

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->startAnswer(se, e, matchState.whoBuzzedName());
        };
        waiting = true;
    };

    void
    handlePASS(ServerEntityManager *serverEntityManager, EntityManager *entityManager, ClientEvent msg)
    {
        if (matchState.roundStage != MatchState::RoundStage::PASS_OR_PLAY)
        {
            return;
        }

        const std::string winner = matchState.whoIsInFaceoffNames()[matchState.faceoffWinner];
        if (msg.source != winner)
        {
            return;
        }

        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::PASS, {}});

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->startPlay(se, e, 1 - matchState.faceoffWinner);
        };
        waiting = true;
    };

    void handlePLAY(ServerEntityManager *serverEntityManager, EntityManager *entityManager, ClientEvent msg)
    {
        if (matchState.roundStage != MatchState::RoundStage::PASS_OR_PLAY)
        {
            return;
        }

        const std::string &winner = matchState.whoIsInFaceoffNames()[matchState.faceoffWinner];
        if (msg.source != winner)
        {
            return;
        }

        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::PLAY, {}});

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->startPlay(se, e, matchState.faceoffWinner);
        };
        waiting = true;
    };

    std::pair<unsigned int, unsigned int> checkAnswer(std::string answer)
    {
        for (int i = 0; i < matchState.answers.size(); ++i)
        {
            const auto &a = matchState.answers[i];
            if (answer == matchState.answers[i].answer)
            {
                return {i + 1, a.score};
            }
        }

        return {0, 0};
    };

    void handleANSWER(ServerEntityManager *serverEntityManager, EntityManager *entityManager, ClientEvent msg)
    {
        // start by checking if the user can answer
        if (matchState.roundStage == MatchState::RoundStage::FACEOFF)
        {
            if (matchState.buzzer == -1) // no one has buzzed
            {
                return;
            }
            else if (matchState.roundPoints == 0 && matchState.incorrects == 0) // no one has answered yet
            {
                // only the buzzer can answer
                if (msg.source != matchState.whoBuzzedName()) // this needs to be in an enclosed if because otherwise fails to allow buzzer to answer
                {
                    return;
                }
            }
            else // someone has buzzed and answered
            {
                // only the other person in the faceoff can answer
                if (msg.source != matchState.whoDidntBuzzName())
                {
                    return;
                }
            }
        }
        else if (matchState.roundStage == MatchState::RoundStage::STEAL)
        {
            // when stealing, only the head of the family can answer
            auto stealingFamily = matchState.families[1 - matchState.playingFamily];
            if (msg.source != stealingFamily.players[0].name)
            {
                return;
            }
        }
        else if (matchState.roundStage != MatchState::RoundStage::PLAY)
        {
            if (msg.source != matchState.whoseTurnName())
            {
                return;
            }
        }
        else
        {
            // can't answer during any of the other stages
            return;
        }

        // TODO(asad): check timer here?
        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::ANSWER, {}, msg.answer, msg.source});

        nextStage = [this, msg](ServerEntityManager *se, EntityManager *e) {
            this->respondToAnswer(se, e, msg.source, msg.answer);
        };
        waiting = true;
    };

    void startRound(ServerEntityManager *serverEntityManager, EntityManager *entityManager)
    {
        // select question
        std::string question = "What is a fucking color?!??"; // TODO(asad): grab this question from bank
        matchState.question = question;
        // populate answers
        matchState.answers = {{{false, "RED", 101}, {false, "BLUE", 79}, {false, "GREEN", 52}, {false, "ORANGE", 44}, {false, "PURPLE", 23}, {false, "YELLOW", 9}, {false, "WHITE", 11}, {false, "", 0}}};
        // DONT SEND EITHER

        MatchState sparseState;
        sparseState.round = matchState.round;
        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::ROUND_START, sparseState});

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->prepareFaceoff(se, e);
        };
        waiting = true;
    };

    void prepareFaceoff(ServerEntityManager *serverEntityManager, EntityManager *entityManager)
    {
        matchState.roundStage = MatchState::RoundStage::FACEOFF;

        MatchState sparseState;
        sparseState.roundStage = matchState.roundStage;
        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::FACEOFF_PREPARE, sparseState});

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->startFaceoff(se, e);
        };
        waiting = true;
    };

    void startFaceoff(ServerEntityManager *serverEntityManager, EntityManager *entityManager)
    {
        MatchState sparseState;
        sparseState.question = matchState.question;
        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::FACEOFF_START, sparseState});
    };

    void startPassOrPlay(ServerEntityManager *serverEntityManager, EntityManager *entityManager, int winner)
    {
        matchState.roundStage = MatchState::RoundStage::PASS_OR_PLAY;
        matchState.faceoffWinner = winner;

        MatchState sparseState;
        sparseState.roundStage = matchState.roundStage;
        sparseState.faceoffWinner = matchState.faceoffWinner;

        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::PASS_OR_PLAY, sparseState});
    };

    void startPlay(ServerEntityManager *serverEntityManager, EntityManager *entityManager, int playingFamily)
    {
        matchState.roundStage = MatchState::RoundStage::PLAY;
        matchState.playingFamily = playingFamily;

        MatchState sparseState;
        sparseState.roundStage = matchState.roundStage;
        sparseState.playingFamily = matchState.playingFamily;

        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::PLAY_START, sparseState});
    };

    void startSteal(ServerEntityManager *serverEntityManager, EntityManager *entityManager)
    {
        matchState.roundStage = MatchState::RoundStage::STEAL;

        MatchState sparseState;
        sparseState.roundStage = matchState.roundStage;
        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::STEAL_START, sparseState});

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->startAnswer(se, e, matchState.families[1 - matchState.playingFamily].players[0].name);
        };
        waiting = true;
    };

    void endRound(ServerEntityManager *serverEntityManager, EntityManager *entityManager)
    {
        matchState.roundStage = MatchState::RoundStage::END;

        MatchState sparseState;
        sparseState.roundStage = matchState.roundStage;
        broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::ROUND_END, sparseState});

        nextStage = [this](ServerEntityManager *se, EntityManager *e) {
            this->startRound(se, e);
        };
        waiting = true;
    };

    void startAnswer(ServerEntityManager *serverEntityManager, EntityManager *entityManager, std::string answerer)
    {
        ServerEvent event{ServerEvent::Type::ANSWER_START};
        event.answerer = answerer;
        broadcastEvent(serverEntityManager, entityManager, event);
    };

    void respondToAnswer(ServerEntityManager *serverEntityManager, EntityManager *entityManager, std::string source, std::string answer)
    {
        bool answerIncorrect = 0; // used to simplify checking which faceoffer is answering
        const auto [answerRank, answerScore] = checkAnswer(answer);
        if (answerRank > 0)
        {
            matchState.answers[answerRank - 1].revealed = true;
            matchState.roundPoints += answerScore;

            MatchState sparseState;
            sparseState.answers[answerRank - 1] = matchState.answers[answerRank - 1];
            sparseState.roundPoints = matchState.roundPoints;
            broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::FLIP, sparseState});
        }
        else
        {
            answerIncorrect = 1;
            matchState.incorrects += 1;

            MatchState sparseState;
            sparseState.incorrects = matchState.incorrects;
            broadcastEvent(serverEntityManager, entityManager, {ServerEvent::Type::INCORRECT, sparseState});
        }

        if (matchState.roundStage == MatchState::RoundStage::FACEOFF)
        {
            if (matchState.roundPoints == 0 && matchState.incorrects == answerIncorrect) // no one has answered yet
            {
                if (answerRank == 1)
                {
                    // buzzer wins
                    nextStage = [this](ServerEntityManager *se, EntityManager *e) {
                        this->startPassOrPlay(se, e, this->matchState.buzzer);
                    };
                    waiting = true;
                }
                else
                {
                    nextStage = [this](ServerEntityManager *se, EntityManager *e) {
                        this->startAnswer(se, e, matchState.whoDidntBuzzName());
                    };
                    waiting = true;
                }
            }
            else
            {
                if (matchState.roundPoints == 0)
                {
                    // both faceoffers failed, move on to next round
                }
                else if (answerScore > matchState.roundPoints)
                {
                    // non-buzzer wins
                    nextStage = [this](ServerEntityManager *se, EntityManager *e) {
                        this->startPassOrPlay(se, e, 1 - this->matchState.buzzer);
                    };
                    waiting = true;
                }
                else
                {
                    // buzzer wins
                    nextStage = [this](ServerEntityManager *se, EntityManager *e) {
                        this->startPassOrPlay(se, e, this->matchState.buzzer);
                    };
                    waiting = true;
                }
            }
        }
        else if (matchState.roundStage == MatchState::RoundStage::STEAL)
        {
            if (answerRank > 0) // right answer
            {
                matchState.scores[1 - matchState.playingFamily] += matchState.roundPoints;
            }
            else // wrong answer
            {
                matchState.scores[matchState.playingFamily] += matchState.roundPoints;
            }

            // always progress round, since stealing team only gets one chance
            nextStage = [this](ServerEntityManager *se, EntityManager *e) {
                this->endRound(se, e);
            };
            waiting = true;
        }
        else
        {
            matchState.currentPlayers[matchState.playingFamily] += 1;

            if (matchState.incorrects == 3)
            {
                nextStage = [this](ServerEntityManager *se, EntityManager *e) {
                    this->startSteal(se, e);
                };
                waiting = true;
            }
            else
            {
                const auto areAllFlipped = [](const std::array<MatchState::AnswerState, 8> &answers) {
                    for (const auto a : answers)
                    {
                        if (a.score != 0 && a.revealed)
                            return true;
                    }
                    return false;
                };
                if (areAllFlipped(matchState.answers))
                {
                    matchState.scores[matchState.playingFamily] += matchState.roundPoints;

                    nextStage = [this](ServerEntityManager *se, EntityManager *e) {
                        this->endRound(se, e);
                    };
                    waiting = true;
                }
            }
        }
    };

    void broadcastEvent(ServerEntityManager *serverEntityManager, EntityManager *entityManager, ServerEvent event)
    {
        for (const auto &[dest, unused] : clientStates)
        {
            serverEntityManager->pendingServerEvents.push({event, dest});
        }
        entityManager->serverEvents.push_back(event);
    };
};

#endif // FEUD_SERVERSYSTEM_HPP
