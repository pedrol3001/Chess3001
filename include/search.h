#pragma once

#include "types.h"
#include "utils.h"
#include "board.h"

#include <deque>




class Search{
        

    public:

        int searchDepth;
        uint64_t nodesCount;

        Search();


        int pvSearch(Board &board, int alpha, int beta, int depth, std::deque<Move> &pline);
        int quiesce(Board &board, int alpha, int beta );
};