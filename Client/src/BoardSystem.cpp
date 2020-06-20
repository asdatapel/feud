#include "BoardSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AnimationDefinition.hpp"
#include "MatchState.hpp"
#include "UserInput.hpp"

BoardSystem::BoardSystem(GraphicsManager *graphicsManager) : graphicsManager(graphicsManager) {}

void createPieceComponents(EntityManager *entityManager, GraphicsManager *graphicsManager, unsigned int entityId)
{
    // TODO(asad): loading data from file should be done in a LoaderSystem
    Model model = Model::load("models/piece.dae");

    sf::Image texture;
    if (!texture.loadFromFile("textures/piece.png"))
    {
        std::cout << "ERRRRPR READING TEXTURE" << std::endl;
    }
    texture.flipVertically();

    sf::Image answerTexture;
    answerTexture.create(1000, 300, sf::Color::Transparent);

    std::vector<Material::Texture> textures;
    textures.push_back(Material::Texture{"base", texture});
    textures.push_back(Material::Texture{"text", answerTexture});

    Material material = graphicsManager->newMaterial(textures);
    Drawable drawable = graphicsManager->newDrawable(&model, "shaders/piece");
    Transform transform{entityId};

    entityManager->materials[entityId] = material;
    entityManager->drawables[entityId] = drawable;
    entityManager->transforms[entityId] = transform;
}

void createScoreComponents(EntityManager *entityManager, GraphicsManager *graphicsManager, unsigned int entityId)
{
    // TODO(asad): loading data from file should be done in a LoaderSystem
    Model model = Model::load("models/score.dae");

    sf::Image texture;
    if (!texture.loadFromFile("textures/score.png"))
    {
        std::cout << "ERRRRPR READING TEXTURE" << std::endl;
    }
    texture.flipVertically();

    sf::Image textTexture;
    textTexture.create(500, 500, sf::Color::Transparent);

    std::vector<Material::Texture> textures;
    textures.push_back(Material::Texture{"base", texture});
    textures.push_back(Material::Texture{"text", textTexture});

    Material material = graphicsManager->newMaterial(textures);
    Drawable drawable = graphicsManager->newDrawable(&model, "shaders/piece");
    Transform transform{entityId};

    entityManager->materials[entityId] = material;
    entityManager->drawables[entityId] = drawable;
    entityManager->transforms[entityId] = transform;
}

RenderTextRequest renderPieceTextRequest(unsigned int entityId, int pieceNum, MatchState::AnswerState answer)
{
    return RenderTextRequest{
        {
            {"fonts/arial.ttf",
             32,
             answer.answer,
             {45, 51}},
            {"fonts/arial.ttf",
             32,
             std::to_string(answer.score),
             {450, 51}},
            {"fonts/arial.ttf",
             32,
             std::to_string(pieceNum),
             {250, 10}},
        },
        entityId,
        1,
    };
}

RenderTextRequest renderNumberRequest(unsigned int entityId, int number, unsigned int texIndex)
{
    return RenderTextRequest{
        {
            {"fonts/arial.ttf",
             32,
             std::to_string(number),
             {125, 65}},
        },
        entityId,
        texIndex,
    };
}

AnimationDefinition initialTransform(unsigned int pos, unsigned int id)
{
    float x = (pos % 2) * 1.01f - 1.01f / 2.f;
    float y = (pos / 2) * -0.15f;
    return AnimationDefinition{id, glm::vec3(x, y, 0), glm::quat({0, 0, 0}), glm::vec3(x, y, 0), glm::quat({0, 0, 0})};
}

AnimationDefinition initialScoreTransform(int which, unsigned int id)
{
    float x = -.9f + (which * 4.f);
    float y = -0.15f;
    return AnimationDefinition{id, glm::vec3(x, y, 0), glm::quat({0, 0, 0}), glm::vec3(x, y, 0), glm::quat({0, 0, 0})};
}

AnimationDefinition centerScoreTransform(unsigned int id)
{
    float x = 1.15f;
    float y = 0.32f;
    return AnimationDefinition{id, glm::vec3(x, y, 0), glm::quat({0, 0, 0}), glm::vec3(x, y, 0), glm::quat({0, 0, 0})};
}

void BoardSystem::init(EntityManager *entityManager)
{
    for (int i = 0; i < entityManager->pieces.size(); ++i)
    {
        auto &p = entityManager->pieces[i];
        p.entityId = entityManager->entities.size();
        entityManager->entities.push_back({p.entityId});

        createPieceComponents(entityManager, graphicsManager, p.entityId);

        entityManager->animationRequests.push(initialTransform(i, p.entityId));
        entityManager->renderTextRequests.push(renderPieceTextRequest(p.entityId, i + 1, MatchState::AnswerState{false, "??????", 13}));
    }

    for (int i = 0; i < entityManager->scoreDisplays.size(); ++i)
    {
        auto &sd = entityManager->scoreDisplays[i];
        sd.entityId = entityManager->entities.size();
        entityManager->entities.push_back({sd.entityId});

        createScoreComponents(entityManager, graphicsManager, sd.entityId);

        entityManager->animationRequests.push(initialScoreTransform(i, sd.entityId));
        entityManager->renderTextRequests.push(renderNumberRequest(sd.entityId, i + 1, 1));
    }

    // top score panel for round score
    auto &csd = entityManager->centerScoreDisplay;
    csd.entityId = entityManager->entities.size();
    entityManager->entities.push_back({csd.entityId});

    createScoreComponents(entityManager, graphicsManager, csd.entityId);

    entityManager->animationRequests.push(centerScoreTransform(csd.entityId));
    entityManager->renderTextRequests.push(renderNumberRequest(csd.entityId, 15, 1));
}

void BoardSystem::update(EntityManager *entityManager)
{
    for (const auto &e : entityManager->serverEvents)
    {
        if (e.type == ServerEvent::Type::FLIP)
        {
            for (int i = 0; i < entityManager->pieces.size(); ++i)
            {
                auto const &p = e.state.answers[i];
                if (p.revealed)
                {
                    auto &piece = entityManager->pieces[i];
                    piece.flipped = true;
                    AnimationDefinition t = initialTransform(i, piece.entityId);
                    t.endRotation = glm::rotate(t.endRotation, glm::radians(-180.f), glm::vec3(1.f, 0.0f, 0.0f));
                    t.length = 100;
                    t.interpolation = AnimationDefinition::EaseOutCubic;
                    entityManager->animationRequests.push(t);

                    entityManager->renderTextRequests.push(renderPieceTextRequest(piece.entityId, i + 1, p));
                }
            }

            entityManager->renderTextRequests.push(renderNumberRequest(entityManager->centerScoreDisplay.entityId, e.state.roundPoints, 1));
        }
        else if (e.type == ServerEvent::Type::ROUND_END)
        {
            for (int i = 0; i < entityManager->pieces.size(); ++i)
            {
                auto &p = entityManager->pieces[i];
                if (p.flipped)
                {
                    p.flipped = false;

                    AnimationDefinition t = initialTransform(i, p.entityId);
                    t.startRotation = glm::rotate(t.endRotation, glm::radians(180.f), glm::vec3(1.f, 0.0f, 0.0f));
                    t.length = 100;
                    t.interpolation = AnimationDefinition::EaseOutCubic;
                    entityManager->animationRequests.push(t);
                }
            }

            for (int i = 0; i < entityManager->scoreDisplays.size(); ++i)
            {
                auto &sd = entityManager->scoreDisplays[i];
                entityManager->renderTextRequests.push(renderNumberRequest(sd.entityId, entityManager->matchState.scores[i], 1));
            }
            entityManager->renderTextRequests.push(renderNumberRequest(entityManager->centerScoreDisplay.entityId, e.state.roundPoints, 1));
        }
    }

    // draw calls
    for (int i = 0; i < entityManager->pieces.size(); ++i)
    {
        auto &p = entityManager->pieces[i];
        entityManager->drawRequests.push(
            DrawRequest{p.entityId, &entityManager->drawables[p.entityId],
                        &entityManager->transforms[p.entityId],
                        "shaders/piece",
                        &entityManager->materials[p.entityId],
                        &entityManager->baseRenderTarget});
    }

    for (int i = 0; i < entityManager->scoreDisplays.size(); ++i)
    {
        auto &sd = entityManager->scoreDisplays[i];
        entityManager->drawRequests.push(
            DrawRequest{sd.entityId, &entityManager->drawables[sd.entityId],
                        &entityManager->transforms[sd.entityId],
                        "shaders/piece",
                        &entityManager->materials[sd.entityId],
                        &entityManager->baseRenderTarget});
    }

    auto &csd = entityManager->centerScoreDisplay;
    entityManager->drawRequests.push(
        DrawRequest{csd.entityId, &entityManager->drawables[csd.entityId],
                    &entityManager->transforms[csd.entityId],
                    "shaders/piece",
                    &entityManager->materials[csd.entityId],
                    &entityManager->baseRenderTarget});
}