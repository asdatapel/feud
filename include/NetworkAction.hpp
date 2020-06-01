#ifndef FEUD_NETWORKACTION_HPP
#define FEUD_NETWORKACTION_HPP

struct NetworkAction
{
    enum struct Type
    {
        RESET,
        MATCH_START,
        MATCH_END,
        ROUND_START,
        ROUND_END,
        FACEOFF_START,
        FACEOFF_END,
        SCORE_UPDATE,
        FLIP,
        INCORRECT,
        GAME_DUMP,
    };

    Type type;
    int flipPiece;
    std::string flipAnswer;
    int incorrectCount;
};

#endif // FEUD_NETWORKACTION_HPP
