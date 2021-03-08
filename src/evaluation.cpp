#include "../include/evaluation.h"



int Evaluation::getBoardScore(Board &board){

    int score=0;
    for(Square sq = SQUARE_ZERO; sq < SQUARE_INIT ;sq++){
        Piece p = board.bitboards.getSquareFromBoard(sq);
        score += pieceScore[p];
        if (p == WHITE) {
            score += positionalScore[typeOfPiece(p)][mirrorBoard[sq]];
        }else {
            score -= positionalScore[typeOfPiece(p)][sq];
        }
    }

    return board.colorToMove == WHITE ? score : -score;

}
