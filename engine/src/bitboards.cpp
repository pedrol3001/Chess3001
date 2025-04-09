#include "../include/bitboards.h"
#include "../include/attacks.h"

// initiate fixed values;
Bitboards::Bitboards(){

    this->clearBoard();
  
}

void Bitboards::setPiece(const Square sq, const Piece piece) {
    assert( this->PieceBoard[sq] == NO_PIECE);
    this->TypeBitboards[ALL_PIECES]                     |= setMask(sq);
    this->TypeBitboards[typeOfPiece(piece)]             |= setMask(sq);
    this->ColorBitboards[colorOfPiece(piece)]           |= setMask(sq);
    this->PieceBoard[sq]                                = piece;
}

void Bitboards::rmPiece(const Square sq, const Piece piece){
    assert( this->PieceBoard[sq] == piece);
    Bitboard target = setMask(sq);
    this->TypeBitboards[ALL_PIECES]                 ^= target;
    this->TypeBitboards[typeOfPiece(piece)]          ^= target;
    this->ColorBitboards[colorOfPiece(piece)]        ^= target;
    this->PieceBoard[sq]                            = NO_PIECE;
}

void Bitboards::movePiece(const Square source,const Square target, const Piece piece) {
    assert( this->PieceBoard[source] == piece);
    Bitboard moveMask = setMask(source) ^ setMask(target);
    this->TypeBitboards[ALL_PIECES]                 ^= moveMask;
    this->TypeBitboards[typeOfPiece(piece)]         ^= moveMask;
    this->ColorBitboards[colorOfPiece(piece)]       ^= moveMask;
    this->PieceBoard[source]                        = NO_PIECE;
    this->PieceBoard[target]                        = piece;
}
// Get pieces color bit board
Bitboard Bitboards::getColorBitboard(const Color C)const {
    return this->ColorBitboards[C];
}

// Get pieces type bitboard;
Bitboard Bitboards::getTypeBitBoard(const PieceType piece)const {
    return this->TypeBitboards[piece];
}

// Get the intercection between color and types bitboards, return the pieces of a color filtered by type
Bitboard Bitboards::getTypeByColorBB(const PieceType piece,const Color C)const {

    return this->TypeBitboards[piece] & this->ColorBitboards[C];

}

Piece Bitboards::getSquareFromBoard(const Square sq)const {

    return this->PieceBoard[sq];

}

Bitboard Bitboards::getPieceBitboard(const Piece piece) const {
    return this->getTypeByColorBB(typeOfPiece(piece),colorOfPiece(piece));
}

bool Bitboards::isKingAttacked(const Color C){

    return this->isSquareAttacked(this->getKingSquare(C),!C);
}

bool Bitboards::isKingAttacked(const Color C, std::vector<Square> &attackers){
    Attacks *attacks = Attacks::GetInstance();

    Square sq = this->getKingSquare(C);

    Bitboard attk_bb = 0ULL;

    attackers.clear();
    // Pawn
    attk_bb |= attacks->getPawnAttack(sq,C == WHITE ? BLACK: WHITE) & this->getTypeByColorBB(PAWN,C);
    //Knight
    attk_bb |= attacks->getNonSliddingAttack(sq,KNIGHT) & this->getTypeByColorBB(KNIGHT,C);
    //Bishop
    attk_bb |= attacks->getSliddingAttack(sq,BISHOP,this->getTypeBitBoard(ALL_PIECES)) & this->getTypeByColorBB(BISHOP,C);
    //Rook
    attk_bb |= attacks->getSliddingAttack(sq,ROOK,this->getTypeBitBoard(ALL_PIECES)) & this->getTypeByColorBB(ROOK,C);
    //Queen
    attk_bb |= attacks->getSliddingAttack(sq,QUEEN,this->getTypeBitBoard(ALL_PIECES)) & this->getTypeByColorBB(QUEEN,C);
    //King
    //attk_bb |= attacks->getNonSliddingAttack(sq,KING) & this->getTypeByColorBB(KING,C);

    while(attk_bb){
        Square atk_square = getLs1b(attk_bb);

        attackers.push_back(std::move(atk_square));

        popLs1b(attk_bb);
    }

    return !attackers.empty();
}

Square Bitboards::getKingSquare(const Color C){
    return getLs1b(this->getTypeByColorBB(KING,C));
}

bool Bitboards::isSquareAttacked(const Square sq, const Color C){


    Attacks *attacks = Attacks::GetInstance();
    //King
    if (attacks->getNonSliddingAttack(sq, KING) & this->getTypeByColorBB(KING, C)) {
        return true;
    }


    // Pawn
    if (attacks->getPawnAttack(sq, !C) & this->getTypeByColorBB(PAWN, C)) {
        return true;
    }


    //Knight
    if (attacks->getNonSliddingAttack(sq, KNIGHT) & this->getTypeByColorBB(KNIGHT, C)) {
        return true;
    }

    //Bishop
    if(attacks->getSliddingAttack(sq,BISHOP,this->getTypeBitBoard(ALL_PIECES)) & this->getTypeByColorBB(BISHOP,C)){
        return true;
    }

    //Rook
    if(attacks->getSliddingAttack(sq,ROOK,this->getTypeBitBoard(ALL_PIECES)) & this->getTypeByColorBB(ROOK,C)){
        return true;
    }

    //Queen
    if(attacks->getSliddingAttack(sq,QUEEN,this->getTypeBitBoard(ALL_PIECES)) & this->getTypeByColorBB(QUEEN,C)){
        return true;
    }


    return false;

}

void Bitboards::clearBoard(){
    memset(this->ColorBitboards,0,sizeof(this->ColorBitboards));
    memset(this->TypeBitboards,0,sizeof(this->TypeBitboards));
    memset(this->PieceBoard,0,sizeof(this->PieceBoard));
}