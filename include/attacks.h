#pragma once

#include "types.h"
#include "utils.h"
#include "magicBits.h"

class Attacks{

private:    

    static Attacks *instance;

    Bitboard PawnSliddingAttacks[static_cast<unsigned long long>(Color::COLOR_INIT)][static_cast<unsigned long long>(Square::SQUARE_INIT)];
    Bitboard NonSliddingAtacks[2][static_cast<unsigned long long>(Square::SQUARE_INIT)]; // KNIGHT and KING
    MagicBits SliddindAtacks;

protected:

    Attacks();

public:

    // Singletom methods
    Attacks(Attacks &other) = delete; // Cant clone 

    void operator=(const Attacks &) = delete; // Cant assign

    static Attacks *GetInstance();

    Bitboard getPawnAttack(const Square sq, const Color C);

    Bitboard getNonSliddingAttack(const Square sq , const PieceType P);

    Bitboard getSliddingAttack(const Square sq, const PieceType P, const Bitboard blockers);
    
    Bitboard pawnAttacks(const Bitboard pawns, const Color C);

    Bitboard knightAttacks(Bitboard knights);

    Bitboard kingAttacks(Bitboard kings);
        
    Bitboard bishopAttacks(const Bitboard blockers, const Square sq);

    Bitboard rookAttacks(const Bitboard blockers, const Square sq);

    Bitboard queenAttacks(const Bitboard blockers, const Square sq);

};