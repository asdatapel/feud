#ifndef FEUD_NETWORKACTION_HPP
#define FEUD_NETWORKACTION_HPP

struct NetworkAction {
    enum struct Type {
        RESET,
        FLIP,
        INCORRECT,
    };

    Type type;
    int flipPiece;
    std::string flipAnswer;
    int incorrectCount;
};

#endif // FEUD_NETWORKACTION_HPP
