#ifndef FEUD_TIMEDEVENT_HPP
#define FEUD_TIMEDEVENT_HPP

struct TimedEvent {
    unsigned int duration;
    bool enabled;
    bool complete;
};

#endif // FEUD_TIMEDEVENT_HPP