#include "../include/attacks.h"

#include <chrono>

// Static members must be declarated outside class body
Attacks* Attacks::instance{nullptr};

// Fill the PawnSlidingAttacks, NonSlidingAttacks and SlidingAttacks buffers
Attacks::Attacks(){

    // Init Pawns, kinghts, and kings arrays
    for(Square s = A1; s < SQUARE_INIT;s++){
        
        Bitboard sq = setMask(s);

        this->NonSliddingAtacks[0][s]=this->knightAttacks(sq);
        this->NonSliddingAtacks[1][s]= this->kingAttacks(sq);
        

        this->PawnSliddingAttacks[BLACK][s] = this->pawnAttacks(sq,BLACK);
        this->PawnSliddingAttacks[WHITE][s] = this->pawnAttacks(sq,WHITE);

    }

}

// Singleton get instance
Attacks *Attacks::GetInstance()
{

    if (instance == nullptr)
    {

        auto precalc_attacks = std::chrono::high_resolution_clock::now();

        instance = new Attacks();

        auto precalc_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - precalc_attacks);

        std::cout << "Pre calculated attacks loaded in: " << precalc_duration.count() << " ms" << std::endl << std::endl;
        
    }
    return instance;
}

Bitboard Attacks::getPawnAttack(const Square sq,const Color C){
    return PawnSliddingAttacks[C][sq];
}

//Return NonSliding pieces attack bitboard by a square and the piece type (KING, KNIGHT) 
//from the NonSliddingAtacks buffer
Bitboard Attacks::getNonSliddingAttack(const Square sq , const PieceType P){


    if(P == KNIGHT || P == KING){
        return this->NonSliddingAtacks[P == KNIGHT ? 0 : 1 ][sq];
    }
    throw std::logic_error("Not a non slidding piece");
}

// Return Slinding pieces attack bitboard by a square, the blockers bitboats 
//and the piece type (ROOK, BISHOP, QUEEN) from the magic bitboard attack buffer
Bitboard Attacks::getSliddingAttack(const Square sq, const PieceType P, const Bitboard blockers){


    switch (P) {
        case BISHOP: return bishopAttacks(blockers, sq);
        case ROOK: return rookAttacks(blockers, sq);
        case QUEEN: return queenAttacks(blockers, sq);
        default: throw std::logic_error("Not a sliding piece");
    }
}

// Geerate a pattern for pawn attacks of a given bitboard and color
Bitboard Attacks::pawnAttacks(const Bitboard pawns , const Color C){
        return C ==  Color::WHITE  ? shift<NORTH_WEST>(pawns) | shift<NORTH_EAST>(pawns) 
                            : shift<SOUTH_WEST>(pawns) | shift<SOUTH_EAST>(pawns);
}

// Geerate a pattern for knights attacks of a given bitboard
Bitboard Attacks::knightAttacks(Bitboard knights) {

   Bitboard l1 = (knights >> 1) & uint64_t(0x7f7f7f7f7f7f7f7fULL);
   Bitboard l2 = (knights >> 2) & uint64_t(0x3f3f3f3f3f3f3f3fULL);
   Bitboard r1 = (knights << 1) & uint64_t(0xfefefefefefefefeULL);
   Bitboard r2 = (knights << 2) & uint64_t(0xfcfcfcfcfcfcfcfcULL);
   Bitboard h1 = l1 | r1;
   Bitboard h2 = l2 | r2;
   return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

// Geerate a pattern for king attacks of a given bitboard
Bitboard Attacks::kingAttacks(Bitboard kings){
    Bitboard attacks = shift<EAST>(kings) | shift<WEST>(kings);
    kings    |= attacks;
    attacks    |= shift<NORTH>(kings) | shift<SOUTH>(kings);
    return attacks;
}

// Get a pattern for bishop attacks from magic bitboad attack buffer get_bishopAttacks()
Bitboard Attacks::bishopAttacks(const Bitboard blockers, const Square sq) {

    Magic m = this->SliddindAtacks.get_bishopMagics(sq);
    Bitboard occupancy = blockers & m.mask;
    const uint64_t index = ((occupancy * m.magic) >> (SQUARE_INIT - m.shift)) + m.offset;
    return this->SliddindAtacks.get_bishopAttacks(unsigned int(index));
}

// Get a pattern for rook attacks from magic bitboad attack buffer
Bitboard Attacks::rookAttacks(const Bitboard blockers, const Square sq) {

    Magic m = this->SliddindAtacks.get_rookMagics(sq);
    Bitboard occupancy = blockers & m.mask;
    const uint64_t index = ((occupancy * m.magic) >> (SQUARE_INIT - m.shift)) + m.offset;
    return this->SliddindAtacks.get_rookAttacks(unsigned int (index));

}

// Get a pettern for quenn attacks by ORing the bishop and rook bitboards
Bitboard Attacks::queenAttacks(const Bitboard blockers, const Square sq) {
    return rookAttacks(blockers, sq) | bishopAttacks(blockers, sq);
}
