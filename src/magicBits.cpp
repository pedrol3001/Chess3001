#include "../include/magicBits.h"
#include "../include/magics.h"


// Init everything
MagicBits::MagicBits(){

  this->bishopAtacks.clear();
  memset(this->bishopMagics,0,sizeof(this->bishopMagics));
  this->rookAtacks.clear();
  memset(this->rookMagics,0,sizeof(this->rookMagics));

  this->rookAtacks.reserve(102400);
  this->bishopAtacks.reserve(5248);

  this->initRookMagics();
  this->initBishopMagics();

}

// Generate a magic number and an attack table relative to a square and the desired directions and shifts;
void MagicBits::GenerateMagic(const Square index,
                        const std::vector<Direction>& directions,
                        const int shift_bits,
                        Bitboard* magic,
                        std::vector<Bitboard>& attack_table) {
    // Generate occupancies.
    std::vector<Bitboard> occupancies;
    for (const Direction& direction : directions) {
      OccupancyCombine(index,direction,occupancies);
    }


    // Generate attacks.
    std::vector<Bitboard> attacks;
    for (const Bitboard occupancy : occupancies) {
      Bitboard attack = 0ULL;
      for (const Direction& direction : directions) {
        attack |= GenerateAttack(index, direction ,occupancy);
      }
      attacks.push_back(std::move(attack));
    }

    // No bishop or rook attack can cover all squares of the board.
    static const Bitboard kInvalidAttack = ~0ULL;

    // Trial and error approach to generate magics.
    while (true) {
      std::vector<Bitboard> table(setMask(Square(shift_bits)), kInvalidAttack);
      Bitboard candidate_magic = ZeroBitBiasedRandom();
      bool collision = false;
      
      for (unsigned int k = 0; k < occupancies.size(); ++k) {
        
        const Bitboard occupancy = occupancies.at(k);
        const Bitboard attack = attacks.at(k);
        const Bitboard offset =
            (occupancy * candidate_magic) >> (64 - shift_bits);
        if (table.at(offset) == kInvalidAttack || table.at(offset) == attack) {
          table.at(offset) = attack;
        } else {
          collision = true;
          break;
        }
      }
      if (!collision) {
        *magic = candidate_magic;
        attack_table.swap(table);
        break;
      }
    }
}

// Generate all magics numbers and atacks of a given set of directions.
// It uses the GenerateMagic() method to every square in a chess board.
void MagicBits::GenerateAllMagics(const std::vector<Direction>& directions,
               const int shifts[],
               Bitboard magics[],
               std::vector<Bitboard>& attack_table,
               int offsets[]) {

  // Loop all squares
  for (Square i = SQUARE_ZERO; i < SQUARE_INIT; ++i) {
    std::vector<Bitboard> tmp_attack_table;
    GenerateMagic(i, directions, shifts[i], &magics[i], tmp_attack_table);
    offsets[i] = static_cast<int>(attack_table.size());
    attack_table.insert(attack_table.end(),
                         tmp_attack_table.begin(),
                         tmp_attack_table.end());
  }
}

// Init all rook atributes relative to magic bitboards
void MagicBits::initRookMagics(){

  Bitboard magic_aux[64];
  int offsets_aux[64];
  std::vector<Direction> directions = {NORTH, SOUTH, WEST,EAST};
  GenerateAllMagics(directions,rook_shifts,magic_aux,this->rookAtacks,offsets_aux);

  for(Square i = SQUARE_ZERO; i<SQUARE_INIT;i++){
    this->rookMagics[i].magic = magic_aux[i];
    this->rookMagics[i].offset = offsets_aux[i];
    this->rookMagics[i].shift = rook_shifts[i];

    for (const Direction& d_aux : directions) {
      this->rookMagics[i].mask |= MaskBits(i,d_aux);
    }
  }

}

// Init all rook atributes relative to magic bitboards
void MagicBits::initBishopMagics(){

  Bitboard magic_aux[64];
  int offsets_aux[64];
  std::vector<Direction> directions = {NORTH_EAST, SOUTH_EAST, NORTH_WEST, SOUTH_WEST};
  GenerateAllMagics(directions,bishop_shifts,magic_aux,this->bishopAtacks,offsets_aux);

  for(Square i = SQUARE_ZERO; i<SQUARE_INIT;i++){
    this->bishopMagics[i].magic = magic_aux[i];
    this->bishopMagics[i].offset = offsets_aux[i];
    this->bishopMagics[i].shift = bishop_shifts[i];

    for (const Direction& d_aux : directions) {
      this->bishopMagics[i].mask |= MaskBits(i,d_aux);
    }
  }
}

// Getters

Magic MagicBits::get_rookMagics(const Square sq){

   return this->rookMagics[sq];
}

Bitboard MagicBits::get_rookAttacks(const unsigned int index){

   return this->rookAtacks[index];
}

Magic MagicBits::get_bishopMagics(const Square sq){
   return this->bishopMagics[sq];
}

Bitboard MagicBits::get_bishopAttacks(const unsigned int index){
   return this->bishopAtacks[index];
}
