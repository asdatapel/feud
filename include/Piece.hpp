#ifndef FEUD_PIECE_HPP
#define FEUD_PIECE_HPP

#include "Drawable.hpp"
#include "GraphicsManager.hpp"

struct Piece
{
    unsigned int entityId;

    char *answer = "hello";
    bool flipped;
};

#endif //FEUD_PIECE_HPP
