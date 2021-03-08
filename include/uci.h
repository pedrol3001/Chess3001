#pragma once

#include "moves.h"
#include "types.h"
#include "utils.h"
#include "board.h"


#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace Uci{

    void position(Board &board, std::istringstream& is);
    void go(Board& board, std::istringstream& is);
    std::string writeMove(const Move move);
    Move parseMove(const std::deque<Move> &moveList, std::string &move);
    void loop(int argc, char* argv[]);

}