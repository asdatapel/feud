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
    std::string whoseTurnName(){
        unsigned int index = currentPlayers[playingFamily] % families[playingFamily].playerCount;
        return families[playingFamily].players[index].name;
    };
    std::array<std::string, 2> whoIsInFaceoffNames()
    {
        int family1Index = round % families[0].playerCount;
        int family2Index = round % families[1].playerCount;
        return {families[0].players[family1Index].name, families[1].players[family2Index].name};
    };
    std::string whoBuzzedName()
    {
        auto faceoffers = whoIsInFaceoffNames();
        return faceoffers[buzzer];
    };
    std::string whoDidntBuzzName()
    {
        auto faceoffers = whoIsInFaceoffNames();
        return faceoffers[1 - buzzer];
    };
    std::string playerName(int team, int index);
};

#endif // FEUD_MATCHSTATE_HPP