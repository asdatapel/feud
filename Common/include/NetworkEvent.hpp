#ifndef FEUD_NETWORKACTION_HPP
#define FEUD_NETWORKACTION_HPP

#include "MatchState.hpp"

struct ServerEvent
{
    enum struct Type
    {
        MATCH_START,
        MATCH_END,
        ROUND_START,
        ROUND_END,
        FACEOFF_PREPARE,
        FACEOFF_START,
        BUZZ,
        PASS_OR_PLAY,
        PASS,
        PLAY,
        PLAY_START,
        STEAL_START,
        ANSWER_START,
        ANSWER,
        FLIP,
        INCORRECT,
    };
    
    Type type;
    
    //TODO(asad): we're wasting data here, use polymorphism or whatever to optimize
    MatchState state;

    std::string answer;
    std::string answerer;
};

struct ClientEvent {
    enum struct Type {
        JOIN,
        BUZZ,
        PLAY,
        PASS,
        READY,
        ANSWER,
    };

    Type type;

    std::string source;

    std::string answer;
};

#endif // FEUD_NETWORKACTION_HPP
