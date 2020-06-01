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
#include "NetworkAction.hpp"
#include "Piece.hpp"
#include "Text.hpp"
#include "UserInput.hpp"

struct EntityManager
{
    std::vector<Entity> entities;

    std::array<Piece, 8> pieces;
    std::array<ScoreDisplay, 2> scoreDisplays;

    std::unordered_map<unsigned int, Drawable> drawables;
    std::unordered_map<unsigned int, Material> materials;
    std::unordered_map<unsigned int, Transform> transforms;
    std::queue<RenderTextRequest> renderTextRequests;
    std::queue<DrawRequest> drawRequests;

    std::queue<AnimationDefinition> animationRequests;
    std::vector<Animation> inprogressAnimations;

    std::vector<UserInput> userActions;
    std::vector<NetworkAction> networkActions;

    // global state
    MatchState matchState;

    // TODO(asad): clean all these resources up
    Material::Texture baseRenderTarget;
    Drawable screenQuad;
    std::string baseShader;
    // TODO(asad): text shader should also be a global var here
    int secondaryFramebuffer;
};

#endif //FEUD_ENTITYMANAGER_HPP