#ifndef FEUD_GAMEPLAYINTERFACESYSTEM_HPP
#define FEUD_GAMEPLAYINTERFACESYSTEM_HPP

#include "EntityManager.hpp"
#include "Model.hpp"
#include "NetworkEvent.hpp"

void showNotification(EntityManager *entityManager, std::string text)
{
    entityManager->renderTextRequests.push(RenderTextRequest{
        {
            {"fonts/arial.ttf",
             48,
             text,
             {0, 10}},
        },
        entityManager->notifications,
        0,
        sf::Color::Red});
    entityManager->timedEvents[entityManager->notifications] = {600, true, false};
};

struct GameplayInterfaceSystem
{
    GameplayInterfaceSystem(GraphicsManager *graphicsManager) : graphicsManager(graphicsManager){};

    void init(EntityManager *entityManager)
    {
        // EEEEeeeeegggghhhhssss
        for (int i = 0; i < entityManager->Xs.size(); ++i)
        {
            auto &x = entityManager->Xs[i];
            x = entityManager->entities.size();
            entityManager->entities.push_back({x});

            sf::Image texture;
            if (!texture.loadFromFile("textures/X.png"))
            {
                std::cout << "ERRRRPR READING TEXTURE" << std::endl;
            }
            texture.flipVertically();

            std::vector<Material::Texture> textures;
            textures.push_back(Material::Texture{"base", texture});

            Material material = graphicsManager->newMaterial(textures);
            Transform transform{x, {0.6 + i * .45, 1., 0}, {}, {0.2f, 0.5f, 1}};

            entityManager->materials[x] = material;
            entityManager->transforms[x] = transform;
            entityManager->timedEvents[x] = {600, false, true};
        }

        {
            // question text
            entityManager->questionText = entityManager->entities.size();
            entityManager->entities.push_back({entityManager->questionText});

            sf::Image texture;
            texture.create(1000, 200, sf::Color::Transparent);
            std::vector<Material::Texture> textures{Material::Texture{"base", texture}};

            Material material = graphicsManager->newMaterial(textures);
            Transform transform{entityManager->questionText, {.6, 1.65, 0}, {}, {0.5f, 0.1f, 1}};

            entityManager->materials[entityManager->questionText] = material;
            entityManager->transforms[entityManager->questionText] = transform;
            entityManager->renderTextRequests.push(RenderTextRequest{
                {
                    {"fonts/arial.ttf",
                     32,
                     "",
                     {0, 0}},
                },
                entityManager->questionText,
                0,
                sf::Color::Transparent});
        }

        {
            // input
            entityManager->input = entityManager->entities.size();
            entityManager->entities.push_back({entityManager->input});

            sf::Image texture;
            texture.create(800, 200, sf::Color::Transparent);
            std::vector<Material::Texture> textures{Material::Texture{"base", texture}};

            Material material = graphicsManager->newMaterial(textures);
            Transform transform{entityManager->input, {.71, 0.11, 0}, {}, {0.7f, 0.1f, 1}};

            entityManager->materials[entityManager->input] = material;
            entityManager->transforms[entityManager->input] = transform;
            entityManager->renderTextRequests.push(RenderTextRequest{
                {
                    {"fonts/arial.ttf",
                     48,
                     "asdasa",
                     {0, 0}},
                },
                entityManager->input,
                0,
                sf::Color::Black});

            entityManager->takingInput = false;
        }

        {
            // notifications text
            entityManager->notifications = entityManager->entities.size();
            entityManager->entities.push_back({entityManager->notifications});

            sf::Image texture;
            texture.create(2000, 800, sf::Color::Transparent);
            std::vector<Material::Texture> textures{Material::Texture{"base", texture}};

            Material material = graphicsManager->newMaterial(textures);
            Transform transform{entityManager->notifications, {1.67, 0.11, 0}, {}, {.25f, 0.1f, 1}};

            entityManager->materials[entityManager->notifications] = material;
            entityManager->transforms[entityManager->notifications] = transform;
            entityManager->timedEvents[entityManager->notifications] = {600, false, true};
            entityManager->renderTextRequests.push(RenderTextRequest{
                {
                    {"fonts/arial.ttf",
                     56,
                     "",
                     {0, 0}},
                },
                entityManager->notifications,
                0,
                sf::Color::Black});
        }

        {
            {
                // pass selector text
                entityManager->passSelector = entityManager->entities.size();
                entityManager->entities.push_back({entityManager->passSelector});

                sf::Image texture;
                texture.create(1000, 250, sf::Color::Transparent);
                std::vector<Material::Texture> textures{Material::Texture{"base", texture}};

                Material material = graphicsManager->newMaterial(textures);
                Transform transform{entityManager->passSelector, {1.67, 1.72, 0}, {}, {.32f, 0.08f, 1}};

                entityManager->materials[entityManager->passSelector] = material;
                entityManager->transforms[entityManager->passSelector] = transform;
                entityManager->renderTextRequests.push(RenderTextRequest{
                    {
                        {"fonts/arial.ttf",
                         56,
                         "PASS (right click)",
                         {0, 0}},
                    },
                    entityManager->passSelector,
                    0,
                    sf::Color::Black});
            }

            {
                // play selector text
                entityManager->playSelector = entityManager->entities.size();
                entityManager->entities.push_back({entityManager->playSelector});

                sf::Image texture;
                texture.create(1000, 250, sf::Color::Transparent);
                std::vector<Material::Texture> textures{Material::Texture{"base", texture}};

                Material material = graphicsManager->newMaterial(textures);
                Transform transform{entityManager->playSelector, {1.67, 1.91, 0}, {}, {.32f, 0.08f, 1}};

                entityManager->materials[entityManager->playSelector] = material;
                entityManager->transforms[entityManager->playSelector] = transform;
                entityManager->renderTextRequests.push(RenderTextRequest{
                    {
                        {"fonts/arial.ttf",
                         56,
                         "PLAY (left click)",
                         {0, 0}},
                    },
                    entityManager->playSelector,
                    0,
                    sf::Color::Black});
            }
        }
    };

    void update(EntityManager *entityManager)
    {
        for (auto &e : entityManager->serverEvents)
        {
            if (e.type == ServerEvent::Type::MATCH_START)
            {
                showNotification(entityManager, "The game has started.");
            }
            else if (e.type == ServerEvent::Type::MATCH_END)
            {
                // TODO(asad): display winner
                showNotification(entityManager, "The game is over. Someone won?");
            }
            else if (e.type == ServerEvent::Type::ROUND_START)
            {
                std::string roundText = "Round " + std::to_string(e.state.round);
                showNotification(entityManager, roundText + " is starting.");
            }
            else if (e.type == ServerEvent::Type::ROUND_END)
            {
                entityManager->renderTextRequests.push(RenderTextRequest{
                    {
                        {"fonts/arial.ttf",
                         32,
                         "",
                         {0, 0}},
                    },
                    entityManager->questionText,
                    0,
                });
                std::string roundText = "Round " + std::to_string(e.state.round);
                showNotification(entityManager, roundText + " is over.");
            }
            else if (e.type == ServerEvent::Type::FACEOFF_PREPARE)
            {
                auto faceoffers = entityManager->matchState.whoIsInFaceoffNames();
                showNotification(entityManager, faceoffers[0] + " and " + faceoffers[1] + " are now facing off");
            }
            else if (e.type == ServerEvent::Type::FACEOFF_START)
            {
                // TODO(asad): display question it (gradually??)
                entityManager->renderTextRequests.push(RenderTextRequest{
                    {
                        {"fonts/arial.ttf",
                         32,
                         e.state.question,
                         {0, 0}},
                    },
                    entityManager->questionText,
                    0,
                });
            }
            else if (e.type == ServerEvent::Type::BUZZ)
            {
                std::string buzzWinner = entityManager->matchState.whoBuzzedName(); //TODO(asad): We should be querying the main state, not the state that cam with the event
                showNotification(entityManager, buzzWinner + "BUZZED!");
            }
            else if (e.type == ServerEvent::Type::PASS_OR_PLAY)
            {
                showNotification(entityManager, entityManager->matchState.whoIsInFaceoffNames()[entityManager->matchState.faceoffWinner] + " is now choosing PASS or PLAY");
            }
            else if (e.type == ServerEvent::Type::PASS)
            {
                showNotification(entityManager, entityManager->matchState.whoIsInFaceoffNames()[entityManager->matchState.faceoffWinner] + " chose to PASS!");
            }
            else if (e.type == ServerEvent::Type::PLAY)
            {
                showNotification(entityManager, entityManager->matchState.whoIsInFaceoffNames()[entityManager->matchState.faceoffWinner] + " chose to PLAY!");
            }
            else if (e.type == ServerEvent::Type::PLAY_START)
            {
                showNotification(entityManager, "The " + entityManager->matchState.families[entityManager->matchState.playingFamily].name + " family is now answering");
            }
            else if (e.type == ServerEvent::Type::STEAL_START)
            {
                showNotification(entityManager, "The " + entityManager->matchState.families[entityManager->matchState.playingFamily].name + " can now steal");
            }
            else if (e.type == ServerEvent::Type::ANSWER_START)
            {
                // TODO(asad): if current user, display input controls here, otherwise display (...)
                // display timer
                if (e.answerer == entityManager->playerName)
                {
                    entityManager->currentInput = "";
                    entityManager->takingInput = true;
                }
                else
                {
                    showNotification(entityManager, e.answerer + " is answering");
                }
            }
            else if (e.type == ServerEvent::Type::ANSWER)
            {
                entityManager->currentInput = "";
                entityManager->takingInput = false;

                showNotification(entityManager, e.answerer + " answered: " + e.answer);
            }
            else if (e.type == ServerEvent::Type::INCORRECT)
            {
                assert(e.state.incorrects <= 3);
                
                for (int i = 0; i < e.state.incorrects; ++i)
                {
                    auto &x = entityManager->Xs[i];
                    entityManager->timedEvents[x] = {600, true, false};
                }
            }
        }

        for (const auto &e : entityManager->userActions)
        {
            if (e.t == UserInput::Type::SUBMIT)
            {
                ClientEvent ce{ClientEvent::Type::ANSWER, entityManager->playerName};
                ce.answer = entityManager->currentInput;
                entityManager->clientEvents.push(ce);

                entityManager->renderTextRequests.push(RenderTextRequest{
                    {
                        {"fonts/arial.ttf",
                         24,
                         "",
                         {0, 0}},
                    },
                    entityManager->input,
                    0,
                });

                if (entityManager->matchState.roundStage == MatchState::RoundStage::PASS_OR_PLAY)
                {
                    // TODO(asad): (preemptively) hide pass or play controls here
                    // send selection to server
                }
            }
            else if (e.t == UserInput::Type::BUZZ)
            {
                if (entityManager->matchState.roundStage == MatchState::RoundStage::FACEOFF)
                {
                    entityManager->clientEvents.push({ClientEvent::Type::BUZZ, entityManager->playerName});
                }
            }
            else if (e.t == UserInput::Type::LETTER_INPUT)
            {
                entityManager->currentInput += e.character;
                entityManager->renderTextRequests.push(RenderTextRequest{
                    {
                        {"fonts/arial.ttf",
                         24,
                         entityManager->currentInput,
                         {0, 0}},
                    },
                    entityManager->input,
                    0,
                });
            }
            else if (e.t == UserInput::Type::BACKSPACE)
            {
                entityManager->currentInput = entityManager->currentInput.substr(0, entityManager->currentInput.size() - 1);
                entityManager->renderTextRequests.push(RenderTextRequest{
                    {
                        {"fonts/arial.ttf",
                         24,
                         entityManager->currentInput,
                         {0, 0}},
                    },
                    entityManager->input,
                    0,
                });
            }
            else if (e.t == UserInput::Type::LEFT_CLICK)
            {
                // TODO(asad): if PASS_OR_PLAY and if player is the buzzer
                entityManager->clientEvents.push({ClientEvent::Type::PLAY, entityManager->playerName});
            }
            else if (e.t == UserInput::Type::RIGHT_CLICK)
            {
                // TODO(asad): if PASS_OR_PLAY and if player is the buzzer
                entityManager->clientEvents.push({ClientEvent::Type::PASS, entityManager->playerName});
            }
            else if (e.t == UserInput::Type::SELECTION)
            {
                // TODO(asad): if stage = PASS_OR_PLAY + current user, change selection
            }
            else if (e.t == UserInput::Type::READY)
            {
                entityManager->clientEvents.push({ClientEvent::Type::READY, "p1"});
            }
        }

        {
            // drawing question
            entityManager->draw2DRequests.push({entityManager->questionText,
                                                &entityManager->transforms[entityManager->questionText],
                                                &entityManager->materials[entityManager->questionText].textures[0],
                                                &entityManager->baseRenderTarget});
        }

        {
            // drawing input
            if (entityManager->takingInput)
            {
                entityManager->draw2DRequests.push({entityManager->input,
                                                    &entityManager->transforms[entityManager->input],
                                                    &entityManager->materials[entityManager->input].textures[0],
                                                    &entityManager->baseRenderTarget});
            }
        }

        {
            // drawing notification
            if (!entityManager->timedEvents[entityManager->notifications].complete)
            {
                entityManager->draw2DRequests.push({entityManager->notifications,
                                                    &entityManager->transforms[entityManager->notifications],
                                                    &entityManager->materials[entityManager->notifications].textures[0],
                                                    &entityManager->baseRenderTarget});
            }
        }

        {
            // TODO(asad): if we're in PASS_OR_PLAY and if we are the buzzer
            if (entityManager->matchState.roundStage == MatchState::RoundStage::PASS_OR_PLAY)
            {
                // drawing pass or play selector
                entityManager->draw2DRequests.push({entityManager->passSelector,
                                                    &entityManager->transforms[entityManager->passSelector],
                                                    &entityManager->materials[entityManager->passSelector].textures[0],
                                                    &entityManager->baseRenderTarget});
                // drawing pass or play selector
                entityManager->draw2DRequests.push({entityManager->playSelector,
                                                    &entityManager->transforms[entityManager->playSelector],
                                                    &entityManager->materials[entityManager->playSelector].textures[0],
                                                    &entityManager->baseRenderTarget});
            }
        }

        // drawing EEEEEeeeggghhhsss
        for (auto const &x : entityManager->Xs)
        {
            if (!entityManager->timedEvents[x].complete)
            {
                entityManager->draw2DRequests.push({x,
                                                    &entityManager->transforms[x],
                                                    &entityManager->materials[x].textures[0],
                                                    &entityManager->baseRenderTarget});
            }
        }
    };

    GraphicsManager *graphicsManager;
};

#endif // FEUD_GAMEPLAYINTERFACESYSTEM_HPP
