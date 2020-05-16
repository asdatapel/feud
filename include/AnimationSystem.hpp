#ifndef FEUD_ANIMATIONSYSTEM_HPP
#define FEUD_ANIMATIONSYSTEM_HPP

#include "EntityManager.hpp"

struct AnimationSystem {
    void apply(EntityManager *entityManager);
};

#endif //FEUD_ANIMATIONSYSTEM_HPP