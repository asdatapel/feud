#ifndef FEUD_MATCHSTATE_HPP
#define FEUD_MATCHSTATE_HPP

#include <array>

struct MatchState
{
    enum struct RoundStage
    {
        START,
        FACEOFF,
        PLAY,
        STEAL,
        END
    };

    struct AnswerState
    {
        bool revealed;
        std::string answer;
        int score;
    };

    struct Player
    {
        std::string name;
        //TODO(asad): appearance related data here
    };

    struct Family {
        std::vector<Player> players;
    };

    std::array<Family, 2> families;

    unsigned int round;
    RoundStage roundStage;
    unsigned int roundPoints;

    int playingFamily;
    int currentPlayer;

    std::array<AnswerState, 8> answers;
    std::array<int, 2> scores;
};

#endif // FEUD_MATCHSTATE_HPP