#ifndef FEUD_TIMEDEVENTSYSTEM_HPP
#define FEUD_TIMEDEVENTSYSTEM_HPP

#include "EntityManager.hpp"

struct TimedEventSystem
{
    void update(EntityManager *entityManager)
    {
        for (auto &e : entityManager->timedEvents)
        {
            if (e.second.enabled)
            {
                e.second.duration -= 1;
                if (e.second.duration == 0)
                {
                    e.second.complete = true;
                }
            }
        }
    };
};

#endif // FEUD_TIMEDEVENTSYSTEM_HPP