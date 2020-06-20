#include "MatchState.hpp"

std::string MatchState::whoseTurnName()
{
    unsigned int index = currentPlayers[playingFamily] % families[playingFamily].playerCount;
    return families[playingFamily].players[index].name;
};
std::array<std::string, 2> MatchState::whoIsInFaceoffNames()
{
    int family1Index = round % families[0].playerCount;
    int family2Index = round % families[1].playerCount;
    return {families[0].players[family1Index].name, families[1].players[family2Index].name};
};
std::string MatchState::whoBuzzedName()
{
    auto faceoffers = whoIsInFaceoffNames();
    return faceoffers[buzzer];
};
std::string MatchState::whoDidntBuzzName()
{
    auto faceoffers = whoIsInFaceoffNames();
    return faceoffers[1 - buzzer];
};