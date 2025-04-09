#pragma once


#include "types.h"
#include "utils.h"


class Bitboards {

private:

    Bitboard TypeBitboards[PIECE_TYPE_INIT];
    Bitboard ColorBitboards[COLOR_INIT];
    Piece PieceBoard[SQUARE_INIT];

public:

    Bitboards();

    Bitboard getColorBitboard(const Color C) const ;

    Bitboard getTypeBitBoard(const PieceType p) const ;

    Bitboard getTypeByColorBB(const PieceType P, Color C)const;

    Bitboard getPieceBitboard(const Piece P)const;

    Piece getSquareFromBoard(const Square sq)const;

    bool isSquareAttacked(const Square sq, const Color C);
    bool isKingAttacked(const Color C);
    bool isKingAttacked(const Color C, std::vector<Square> &attackers);
    Square getKingSquare(const Color C);
    
    void setPiece(const Square S, const Piece P);
    void rmPiece(const Square S, const Piece P);
    void movePiece(const Square source, const Square target, const Piece P);
    void clearBoard();

};
