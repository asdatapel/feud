#ifndef FEUD_PIECE_HPP
#define FEUD_PIECE_HPP

#include "Drawable.hpp"
#include "GraphicsManager.hpp"

struct Piece
{
    unsigned int entityId;

    bool flipped;
};

struct ScoreDisplay
{
    unsigned int entityId; // this should be the team????

    int score;
};

#endif //FEUD_PIECE_HPP
