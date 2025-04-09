#pragma once

#include "types.h"
#include "utils.h"
#include "bitboards.h"

typedef struct boardHistoryTag{
    Move move;
    Piece capturedPiece;
    CastlingRights castlingRights;
    Square passantSquare;
    
}boardHistory;

class Board{


    public:
        Bitboards bitboards;
        Color colorToMove;
        CastlingRights castlingRights;
        Square passantSquare;
        int halfMoveCounter;
        int fullMoveClock;

        std::vector<boardHistory> history;
        
        // Init and prints
        Board();
        Board(const std::string &fen);
        void printBoard();

        // Getters and setters
        bool setFromFEN(const std::string &FEN_string);
        std::string getFEN() const;
        void clearBoard();
        Piece getCurrentTurnPiece(const PieceType pt)const;

        void updateCastlingRights(const Square sq);

        boardHistory popHistory();
        void pushHistory(const boardHistory &h);
        boardHistory generateBoardHitory(const Move m, const Piece capturedPiece);

        friend std::ostream& operator<<(std::ostream& os, const Board& dt);

};
