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
#include "Model.hpp"
#include "NetworkAction.hpp"
#include "Piece.hpp"
#include "Text.hpp"
#include "UserInput.hpp"

struct EntityManager
{
    std::vector<Entity> entities;

    std::array<Piece, 8> pieces;

    std::queue<NewDrawable> newDrawables;
    std::queue<RenderTextRequest> renderTextRequests;
    std::unordered_map<unsigned int, Drawable> drawables;

    std::queue<AnimationDefinition> animationRequests;
    std::vector<Animation> inprogressAnimations;

    std::vector<UserInput> userActions;
    std::vector<NetworkAction> networkActions;
};

#endif //FEUD_ENTITYMANAGER_HPP