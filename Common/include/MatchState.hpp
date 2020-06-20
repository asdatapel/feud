#ifndef FEUD_MATCHSTATE_HPP
#define FEUD_MATCHSTATE_HPP

#include <array>

struct MatchState
{
    enum struct RoundStage
    {
        START,
        FACEOFF,
        PASS_OR_PLAY,
        PLAY,
        STEAL,
        END
    };

    struct AnswerState
    {
        bool revealed = false;
        std::string answer;
        int score = 0;
    };

    struct Player
    {
        std::string name;
        //TODO(asad): appearance related data here
    };

    struct Family
    {
        std::string name;
        std::array<Player, 6> players;
        size_t playerCount;
    };

    std::array<Family, 2> families;

    unsigned int round;
    RoundStage roundStage;
    unsigned int roundPoints;
    int buzzer;
    int faceoffWinner;

    int playingFamily;
    std::array<int, 2> currentPlayers;

    std::string question;
    std::array<AnswerState, 8> answers;
    std::array<int, 2> scores;
    unsigned int incorrects;

    // helper functions
    std::string whoseTurnName();
    std::array<std::string, 2> whoIsInFaceoffNames();
    std::string whoBuzzedName();
    std::string whoDidntBuzzName();
    std::string playerName(int team, int index);
};

#endif // FEUD_MATCHSTATE_HPP