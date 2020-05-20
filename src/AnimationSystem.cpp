#include "AnimationSystem.hpp"

void AnimationSystem::apply(EntityManager *entityManager)
{
    while (!entityManager->animationRequests.empty())
    {
        auto request = entityManager->animationRequests.front();

        entityManager->inprogressAnimations.push_back({request, 0});

        entityManager->animationRequests.pop();
    }

    for (auto it = entityManager->inprogressAnimations.begin(); it != entityManager->inprogressAnimations.end();)
    {
        const AnimationDefinition &animationDef = it->def;
        Drawable *d = &entityManager->drawables[animationDef.entityId];
        d->transform.position = animationDef.endPosition;
        d->transform.rotation = glm::slerp(animationDef.startRotation, animationDef.endRotation, animationDef.interpolation(it->progress));
        it->progress += 1.f / animationDef.length;

        if (it->progress >= 1.f)
            it = entityManager->inprogressAnimations.erase(it);
        else
            ++it;
    }
}