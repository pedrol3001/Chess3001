#pragma once

#include "board.h"
#include "types.h"
#include "utils.h"
#include "attacks.h"
#include <deque>


namespace MoveGen {


    template <MoveType MT>
    void generatePseudoMoves(Board& board, std::deque<Move>& moveList);

    template <MoveType MT>
    void generateLegalMoves(Board& board, std::deque<Move>& moveList);

    void generatePseudoCastlingMoves(Board& board, std::deque<Move>& moveList);

    template <MoveType MT>
    void generatePseudoPawnMoves(Board& board, std::deque<Move>& moveList);

    template <MoveType MT>
    void generatePseudoKnightMoves(Board& board, std::deque<Move>& moveList);

    template <MoveType MT>
    void generatePseudoKingMoves(Board& board, std::deque<Move>& moveList);

    template <MoveType MT>
    void generatePseudoRookMoves(Board& board, std::deque<Move>& moveList);

    template <MoveType MT>
    void generatePseudoBishopMoves(Board& board, std::deque<Move>& moveList);

    template <MoveType MT>
    void generatePseudoQueenMoves(Board& board, std::deque<Move>& moveList);

};