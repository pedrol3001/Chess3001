#pragma once

#include "board.h"
#include "types.h"
#include "utils.h"
#include <deque>
#include "attacks.h"

namespace Moves{
    

    bool makeMove(Board& board, const Move move);

    void unmakeMove(Board &board);
    

}
