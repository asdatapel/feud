#ifndef FEUD_ANIMATION_HPP
#define FEUD_ANIMATION_HPP

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "AnimationDefinition.hpp"

struct Animation
{
    AnimationDefinition def;
    float progress;
};

#endif //FEUD_ANIMATION_HPP