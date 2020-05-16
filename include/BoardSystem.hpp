#ifndef FEUD_BOARDSYSTEM_HPP
#define FEUD_BOARDSYSTEM_HPP

#include "EntityManager.hpp"
#include "GraphicsManager.hpp"

struct BoardSystem
{
    void init(EntityManager *entityManager, GraphicsManager *graphicsManager, int shaderHandle);
    void update(EntityManager *entityManager);
};

#endif //FEUD_BOARDSYSTEM_HPP
