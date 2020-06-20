#ifndef FEUD_BOARDSYSTEM_HPP
#define FEUD_BOARDSYSTEM_HPP

#include "EntityManager.hpp"
#include "GraphicsManager.hpp"

struct BoardSystem
{
    BoardSystem(GraphicsManager *graphicsManager);
    void init(EntityManager *entityManager);
    void update(EntityManager *entityManager);

    GraphicsManager *graphicsManager;
};

#endif //FEUD_BOARDSYSTEM_HPP
