#include "../include/search.h"
#include "../include/moves.h"
#include "../include/moveGen.h"
#include "../include/evaluation.h"


Search::Search() {
    this->searchDepth = 1;
    this->nodesCount = 0;
}


int Search::pvSearch(Board &board, int alpha, int beta, int depth, std::deque<Move>& pline) {

    std::deque<Move> line;

    if (depth == 0) return quiesce(board, alpha, beta);

    bool bSearchPv = true;

    std::deque<Move> moveList;

    this->nodesCount++;

    MoveGen::generateLegalMoves<ALL_MOVES>(board, moveList);


    for (Move m : moveList) {
        Moves::makeMove(board, m);
        int score;
        if (bSearchPv) {
            score = -pvSearch(board,-beta, -alpha, depth - 1,line);
        }
        else {
            score = -pvSearch(board,-alpha - 1, -alpha, depth - 1, line);
            if (score > alpha) // in fail-soft ... && score < beta ) is common
                score = -pvSearch(board,-beta, -alpha, depth - 1, line); // re-search
        }
        Moves::unmakeMove(board);
        if (score >= beta)
            return beta;   // fail-hard beta-cutoff
        if (score > alpha) {
            alpha = score; // alpha acts like max in MiniMax
            pline.clear();
            pline.push_back(m);
            std::copy(line.begin(), line.end(), std::back_inserter(pline));
            bSearchPv = false;  // *1)
        }
    }

    if (moveList.empty()) {
        if (board.bitboards.isKingAttacked(board.colorToMove)) {
            return (-INF +1000) + board.halfMoveCounter;
        }
        else {
            return 0;
        }
    }
        

    return alpha; // fail-hard

        
}


int Search::quiesce(Board &board, int alpha, int beta ) {

  
    int stand_pat = Evaluation::getBoardScore(board);
    if (stand_pat >= beta)
        return beta;
    if (alpha < stand_pat)
        alpha = stand_pat;

    std::deque<Move> captureMoves;

    MoveGen::generateLegalMoves<CAPTURE_MOVES>(board, captureMoves);

    this->nodesCount++;

    for (Move m : captureMoves) {
        Moves::makeMove(board, m);
        int score = -quiesce(board, -beta, -alpha);
        Moves::unmakeMove(board);

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;

}