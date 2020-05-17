#ifndef FEUD_ANIMATIONDEFINITION_HPP
#define FEUD_ANIMATIONDEFINITION_HPP

#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

struct AnimationDefinition
{
    unsigned int enitity_id;

    glm::vec3 startPosition;
    glm::quat startRotation;
    glm::vec3 endPosition;
    glm::quat endRotation;
    float length;

    std::function<float(float)> interpolation = Linear; // should take a value 0.f-1.f and return 0.f-1.f

    static float Linear(float t) { return t; };
    static float Exponential(float t) { return t * t; };
    static float EaseOutElastic(float t)
    {
        const float c4 = (2 * 3.1415) / 3;

        return t == 0.f ? 0 : t == 1 ? 1 : pow(2, -10 * t) * sin((t * 10 - 0.75f) * c4) + 1;
    };
    static float EaseOutCubic(float t)
    {
        return 1 - std::pow(1 - t, 3);
    }
};

#endif //FEUD_ANIMATIONDEFINITION_HPP