#pragma once

#include "types.h"
#include "utils.h"

struct Magic {
   Bitboard mask;  // to mask relevant squares of both lines (no outer squares)
   Bitboard magic; // magic 64-bit factor
   unsigned offset;
   unsigned shift;
};


// return the index of the next square along this direction. Returns -1 if 
// next index is outside the board.
constexpr Square NextIndex(const Square sq, const Direction D) {
    int row = rank_of(sq);
    int col = file_of(sq);

    switch (D) {
      case NORTH:      ++row;        break;
      case SOUTH:      --row;        break;
      case EAST:       ++col;        break;
      case WEST:       --col;        break;
      case NORTH_EAST: ++row; ++col; break;
      case NORTH_WEST: ++row; --col; break;
      case SOUTH_EAST: --row; ++col; break;
      case SOUTH_WEST: --row; --col; break;
    }
    return (row > 7 || col > 7 || row < 0 || col < 0) ? SQUARE_NONE : Square(Indx(File(col), Rank(row)));
  }


template<typename T1 = Square> inline int distance(Square x, Square y);
template<> inline int distance<File>(Square x, Square y) { return std::abs(file_of(x) - file_of(y)); }
template<> inline int distance<Rank>(Square x, Square y) { return std::abs(rank_of(x) - rank_of(y)); }
template<> inline int distance<Square>(Square x, Square y) { return std::max(distance<File>(x, y), distance<Rank>(x, y)); }

// Masks all the bits from the given index, and along the given direction to 1,
// excluding the square given by the index and the edge of the board along
// given direction.

Bitboard inline MaskBits(const Square index, const Direction D) {
  Bitboard bitboard = 0ULL;
  Square next_index = index;
  while ((next_index = NextIndex(Square(next_index),D)) >= 0 &&
         NextIndex(Square(next_index), D) >= 0) {
    bitboard |= setMask(Square(next_index));
  }
  return bitboard;
}
// Generate all piece occupancies along a rank, file or diagonal, in the
// given direction, with index as the reference point. The square given
// by the index and the edge of the board in the given direction are not
// covered. For example, direction = NORTH_WEST, index = 29 (marked by X)
// will generate all combinations of occupancies for squares marked by #


void inline GenerateOccupancies(const Square index, const Direction D ,std::vector<Bitboard>& bbv) {

  // Number of squares in this direction excluding current square and
  // edge of the board.
  const int num_squares = EdgeDistance(index, D) - 1;
  if (num_squares <= 0) {
    return;
  }

  // Number of possible piece occupancies in these squares along
  // the given direction.
  const unsigned num_occupancies = (1ULL << num_squares);

  // Create bitboard for each occupancy with the index next to given
  // index as starting point, along the given direction.
  for (unsigned occupancy = 0U; occupancy < num_occupancies; ++occupancy) {
    Bitboard bitboard = 0ULL;
    int next_index = index;
    for (unsigned bit_mask = 1U; bit_mask <= occupancy; bit_mask <<= 1) {
      next_index = NextIndex(Square(next_index),D);
      #ifdef DEBUG
      assert(next_index != -1);
      #endif
      bitboard |= (Bitboard(!!(occupancy & bit_mask)) << next_index);
    }
    bbv.push_back(bitboard);
  }
}


void inline OccupancyCombine(const Square index, const Direction D ,std::vector<Bitboard>& occupancies) {
  std::vector<Bitboard> bbv;
  GenerateOccupancies(index, D ,bbv);
  if (bbv.empty()) {
    return;
  }
  if (occupancies.empty()) {
    occupancies.insert(occupancies.end(), bbv.begin(), bbv.end());
    return;
  }
  std::vector<Bitboard> tmp;
  for (const Bitboard bb : bbv) {
    for (const Bitboard occupancy : occupancies) {
      tmp.push_back(bb | occupancy);
    }
  }
  occupancies.swap(tmp);
}



// Generate an attack bitboard from a given square in the given direction
// for a specific occupancy of pieces.
Bitboard inline GenerateAttack(const Square index,const Direction D, const Bitboard occupancy) {
  Bitboard attack_bb = 0ULL;
  for (Square i = index; (i = NextIndex(i,D)) != -1;) {
    attack_bb |= setMask(i);
    if (occupancy & setMask(i)) {
      break;
    }
  }
  return attack_bb;
}

// Class to generate and store all magic numbers, masks , shifts and ofsets.

class MagicBits{

private:

Magic rookMagics[64];
Magic bishopMagics[64];

std::vector<Bitboard> rookAtacks;
std::vector<Bitboard> bishopAtacks;

public:

void inline GenerateMagic(const Square index,
                   const std::vector<Direction>& directions,
                   const int shift_bits,
                   Bitboard* magic,
                   std::vector<Bitboard>& attack_table);


void inline GenerateAllMagics(const std::vector<Direction>& directions,
               const int shifts[],
               Bitboard magics[],
               std::vector<Bitboard>& attack_table,
               int offsets[]);

MagicBits();

void initRookMagics();

void initBishopMagics();

Magic get_rookMagics(const Square sq);
Bitboard get_rookAttacks(const uint64_t index);

Magic get_bishopMagics(const Square sq);
Bitboard get_bishopAttacks(const uint64_t index);

};


