#ifndef FEUD_ENTITYMANAGER_HPP
#define FEUD_ENTITYMANAGER_HPP

#include <array>
#include <queue>
#include <vector>
#include <unordered_map>

#include "Drawable.hpp"
#include "Entity.hpp"
#include "Piece.hpp"
#include "Animation.hpp"
#include "UserAction.hpp"

const int MAX_ENTITIES = 32;

struct EntityManager
{
    std::vector<Entity> entities;

    std::array<Piece, 8> pieces;
    std::unordered_map<unsigned int, Drawable> drawables;
    std::queue<Animation> animations;

    std::vector<UserAction> userActions;
};

#endif //FEUD_ENTITYMANAGER_HPP