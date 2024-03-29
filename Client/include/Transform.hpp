#ifndef FEUD_TRANSFORM_HPP
#define FEUD_TRANSFORM_HPP

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

struct Transform
{
    unsigned int entityId;

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale = {1, 1, 1};
};

#endif //FEUD_TRANSFORM_HPP