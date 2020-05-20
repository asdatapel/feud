#include "BoardSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AnimationDefinition.hpp"
#include "UserInput.hpp"

NewDrawable newDrawable(unsigned int entityId)
{
    // TODO(asad): loading data from file should be done in a LoaderSystem
    Model model = Model::load("models/piece.dae");

    sf::Image texture;
    if (!texture.loadFromFile("textures/piece.png"))
    {
        std::cout << "ERRRRPR READING TEXTURE" << std::endl;
    }
    texture.flipVertically();

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        std::cout << "error loading font" << std::endl;
    }
    sf::Text answerText;
    answerText.setFont(font);
    answerText.setString("REAL ANSWER");

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "uploadTexture end: " << err << std::endl;
    }

    sf::Image answerTexture;
    answerTexture.create(1000, 300, sf::Color(0, 0, 0, 0));

    std::vector<sf::Image> textures;
    textures.push_back(texture);
    textures.push_back(answerTexture);

    return NewDrawable{entityId, model, textures, "shaders/piece"};
}

RenderTextRequest renderTextRequest(unsigned int entityId, std::string text)
{
    //TODO(asad): no new!!!!
    Font *f = new Font;
    f->init("fonts/arial.ttf", 32);
    return RenderTextRequest{
        f,
        entityId,
        1,
        text,
        {50, 51}};
}

AnimationDefinition initialTransform(unsigned int pos, unsigned int id)
{
    float x = (pos % 2) * 1.01f;
    float y = (pos / 2) * -0.15f;
    return AnimationDefinition{id, glm::vec3(x, y, 0), glm::quat({0, 0, 0}), glm::vec3(x, y, 0), glm::quat({0, 0, 0})};
}

void BoardSystem::init(EntityManager *entityManager)
{
    std::string testAnswers[8] = {
        "Red", "Blue", "Green", "Oragne", "Purple", "Yellow", "White", "Bloack"
    }; 

    for (int i = 0; i < entityManager->pieces.size(); ++i)
    {
        auto &p = entityManager->pieces[i];
        p.entityId = entityManager->entities.size();
        entityManager->entities.push_back({p.entityId});
        entityManager->newDrawables.push(newDrawable(p.entityId));
        entityManager->renderTextRequests.push(renderTextRequest(p.entityId, testAnswers[i]));
        entityManager->animationRequests.push(initialTransform(i, p.entityId));
    }

}

void BoardSystem::update(EntityManager *entityManager)
{
    for (const auto &e : entityManager->userActions)
    {
        if (e.t == UserInput::Type::TEST_ACTION)
        {
            if (e.number >= 1 && e.number <= 8)
            {
                auto piece = entityManager->pieces[e.number - 1];
                AnimationDefinition t = initialTransform(e.number - 1, piece.entityId);
                t.endRotation = glm::rotate(t.endRotation, glm::radians(-180.f), glm::vec3(1.f, 0.0f, 0.0f));
                t.length = 100;
                t.interpolation = AnimationDefinition::EaseOutCubic;
                entityManager->animationRequests.push(t);
            }
        }
    }
}