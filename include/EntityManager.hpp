#ifndef FEUD_ENTITYMANAGER_HPP
#define FEUD_ENTITYMANAGER_HPP

#include <array>
#include <queue>
#include <vector>
#include <unordered_map>

#include "Animation.hpp"
#include "AnimationDefinition.hpp"
#include "Drawable.hpp"
#include "Entity.hpp"
#include "MatchState.hpp"
#include "Model.hpp"
#include "NetworkEvent.hpp"
#include "Piece.hpp"
#include "Text.hpp"
#include "TimedEvent.hpp"
#include "UserInput.hpp"

struct EntityManager
{
    std::vector<Entity> entities;

    // components
    std::unordered_map<unsigned int, Drawable> drawables;
    std::unordered_map<unsigned int, Material> materials;
    std::unordered_map<unsigned int, Transform> transforms;

    // push buffers
    std::queue<RenderTextRequest> renderTextRequests;
    std::queue<DrawRequest> drawRequests;
    std::queue<Draw2DRequest> draw2DRequests;

    std::queue<AnimationDefinition> animationRequests;
    std::vector<Animation> inprogressAnimations;

    std::vector<UserInput> userActions;

    // network state
    std::string playerName;
    std::vector<ServerEvent> serverEvents;
    std::queue<ClientEvent> clientEvents;

    // TODO(asad): how to access event without saving pointer??
    // TODO(asad): how to clean up finished events?
    std::unordered_map<unsigned int, TimedEvent> timedEvents;

    // Board state
    std::array<Piece, 8> pieces;
    std::array<ScoreDisplay, 2> scoreDisplays;
    ScoreDisplay centerScoreDisplay;

    //GameInterface state
    unsigned int questionText;
    std::array<unsigned int, 3> Xs;

    unsigned int notifications;

    unsigned int playSelector;
    unsigned int passSelector;

    unsigned int input;
    std::string currentInput;
    bool takingInput;

    // misc state
    MatchState matchState;

    // TODO(asad): clean all these resources up
    Material::Texture baseRenderTarget;
    Drawable screenQuad;
    std::string baseShader;
    std::string shader2D;
    // TODO(asad): text shader should also be a global var here
    int secondaryFramebuffer;
};

#endif //FEUD_ENTITYMANAGER_HPP