#include "AnimationSystem.hpp"

void AnimationSystem::apply(EntityManager *entityManager)
{
    while (!entityManager->animations.empty()){
        auto animation = entityManager->animations.front();

        Drawable *d = &entityManager->drawables[animation.enitity_id];
        d->transform = animation.end;
        
        entityManager->animations.pop();
    }
}