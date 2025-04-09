#pragma once


#include "types.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <chrono>

#include <iostream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

constexpr Square Indx(const File f, const Rank r) { 
    return Square(r * 8 + f); 
}

// File of a square
constexpr File file_of(const Square s) {
    return File(s & 7);
}

// Rank of a square
constexpr Rank rank_of(const Square s) {
     return Rank(s >> 3);
}

constexpr bool squaresOnSameFile(Square sq1, Square sq2) {
    return (file_of(sq1) == file_of(sq2));
}

constexpr bool squaresOnSameRank(Square sq1, Square sq2) {
    return (rank_of(sq1) == rank_of(sq2));
}

constexpr bool squaresOnSameDiagonal(Square sq1, Square sq2) {
    return ((file_of(sq2) - file_of(sq1)) == (rank_of(sq2) - rank_of(sq1)));
}

constexpr bool squaresOnSameAntiDiagonal(Square sq1, Square sq2) {
  return (((file_of(sq2) - file_of(sq1)) + (rank_of(sq2) - rank_of(sq1))) == 0);
}
// Return a rank filed with 1s
constexpr Bitboard rankMask(const Rank rank) {return 0x00000000000000ffULL << (rank * 8);}

// Return a file filed with 1s
constexpr Bitboard fileMask(const File file) {return 0x0101010101010101ULL << (file & 7);}

// Return bitboard with bit in square
constexpr Bitboard setMask(const Square sq){
    return (0x1ULL << sq);
}

// Return bitboard filled with 1 and a 0 in the squere
constexpr Bitboard clearMask(const Square sq){
    return ~(0x1ULL << sq);
}


// Return a diagonal filed with 1s
constexpr Bitboard diagonalMask(const Square sq) {
   const Bitboard maindia = 0x8040201008040201ULL;
   int diag =8*(sq & 7) - (sq & 56);
   int nort = -diag & ( diag >> 31);
   int sout =  diag & (-diag >> 31);
   return (maindia >> sout) << nort;
}

// Return a anti-diagonal filed with 1s
constexpr Bitboard antiDiagMask(const Square sq) {
   const Bitboard maindia = 0x0102040810204080ULL;
   int diag =56- 8*(sq&7) - (sq&56);
   int nort = -diag & ( diag >> 31);
   int sout =  diag & (-diag >> 31);
   return (maindia >> sout) << nort;
}

  // return the number of squares from given square to the edge of the board
  // along this direction.

  template <MoveEncode M>
  constexpr auto decodeMove(Move move){

    if constexpr(M == SOURCE_SQUARE) 
      return Square(move & SOURCE_SQUARE);
    else if constexpr( M == TARGET_SQUARE) 
      return Square((move & TARGET_SQUARE) >> 6);
    else if constexpr( M == PIECE) 
      return Piece((move & PIECE) >> 12);
    else if constexpr( M == PROMOTED_PIECE) 
      return Piece((move & PROMOTED_PIECE) >> 16);
    else if constexpr( M == CAPTURE) 
      return bool((move & CAPTURE) >> 20);
    else if constexpr( M == DOUBLE_PUSH) 
      return bool((move & DOUBLE_PUSH) >> 21);
    else if constexpr( M == ENPASSANT) 
      return bool((move & ENPASSANT) >> 22);
    else if constexpr( M == CASTLING) 
      return bool((move & CASTLING) >> 23);
    else
      return 0x00ULL;
  
  }

  
  constexpr Move encodeMove(
    Square source_square,  Square target_square,
    Piece piece, Piece promoted_piece,
    bool capture, bool double_push, bool enpassant, bool castling){

      return  (source_square) |
              (target_square << 6) |
              (piece << 12) |
              (promoted_piece << 16) |
              (capture << 20) |
              (double_push << 21) |
              (enpassant << 22) |
              (castling << 23);
              // 8 bits 24 - 31 free for improvments;
    }

    // Return Bifboard shifted once in a given direction
  template<Direction D>
  constexpr Bitboard  shift(const Bitboard b) {
    constexpr Bitboard FileHBitboard = fileMask(FILE_H);
    constexpr Bitboard FileABitboard = fileMask(FILE_A);

    return  D == NORTH      ?  b             << 8 : D == SOUTH      ?  b             >> 8
            : D == NORTH+NORTH?  b             <<16 : D == SOUTH+SOUTH?  b             >>16
            : D == EAST       ? (b & ~FileHBitboard) << 1 : D == WEST       ? (b & ~FileABitboard) >> 1
            : D == NORTH_EAST ? (b & ~FileHBitboard) << 9 : D == NORTH_WEST ? (b & ~FileABitboard) << 7
            : D == SOUTH_EAST ? (b & ~FileHBitboard) >> 7 : D == SOUTH_WEST ? (b & ~FileABitboard) >> 9
            : 0;
  }



  constexpr int EdgeDistance(const Square sq , const Direction D ) {
    using std::min;
    const int row = rank_of(sq);
    const int col = file_of(sq);

    auto inv = [](int x)->int { return 7 - x; };

    int d = -1;
    switch (D) {
      case NORTH:      d = inv(row);                break;
      case SOUTH:      d = row;                     break;
      case EAST:       d = inv(col);                break;
      case WEST:       d = col;                     break;
      case NORTH_EAST: d = min(inv(row), inv(col)); break;
      case NORTH_WEST: d = min(inv(row), col);      break;
      case SOUTH_EAST: d = min(row, inv(col));      break;
      case SOUTH_WEST: d = min(row, col);           break;
    }
    assert(d >= 0 && d <= 7);
    return d;
  }

// Returns a unsigned 64 bit random number.
uint64_t inline U64Rand() {
  return (uint64_t(0xFFFF & rand()) << 48) |
      (uint64_t(0xFFFF & rand()) << 32) |
      (uint64_t(0xFFFF & rand()) << 16) |
      uint64_t(0xFFFF & rand());
}

uint64_t inline ZeroBitBiasedRandom() {
  return U64Rand() & U64Rand() & U64Rand();
}

constexpr bool is_valid(const Square s){
   
  return s >= A1 && s <= H8;

} 

// Count number os 1s in bitboard
constexpr int popCount( Bitboard  B){
  uint64_t count = 0; 
    while (B) { 
        count += B & 1; 
        B >>= 1; 
    } 
    return count; 
}

// Count number os 1s in bitboard
constexpr void popLs1b(Bitboard  &b){
 b &= (b-1);
}

#if defined(__GNUC__)  // GCC, Clang, ICC

inline Square getLs1b(Bitboard b) {
  assert(b);
  return Square(__builtin_ctzll(b));
}

inline Square getMs1b(Bitboard b) {
  assert(b);
  return Square(63 ^ __builtin_clzll(b));
}

#elif defined(_MSC_VER)  // MSVC

#ifdef _WIN64  // MSVC, WIN64

inline Square getLs1b(Bitboard b) {
  assert(b);
  uint64_t idx;
  _BitScanForward64(&idx, b);
  return Square(idx);
}

inline Square getMs1b(Bitboard b) {
  assert(b);
  uint64_t idx;
  _BitScanReverse64(&idx, b);
  return (Square) idx;
}

#else  // MSVC, WIN32

inline Square getLs1b(Bitboard b) {
  assert(b);
  uint64_t idx;

  if (b & 0xffffffff) {
      _BitScanForward(&idx, int32_t(b));
      return Square(idx);
  } else {
      _BitScanForward(&idx, int32_t(b >> 32));
      return Square(idx + 32);
  }
}

inline Square getMs1b(Bitboard b) {
  assert(b);
  uint64_t idx;

  if (b >> 32) {
      _BitScanReverse(&idx, int32_t(b >> 32));
      return Square(idx + 32);
  } else {
      _BitScanReverse(&idx, int32_t(b));
      return Square(idx);
  }
}

#endif

#else  // Compiler is neither GCC nor MSVC compatible

#error "Compiler not supported."

#endif

constexpr void popBit(const Square sq, Bitboard &b){

  b = b & clearMask(sq);

}

// Get the bit in square position
constexpr int getBit(const Square sq, const Bitboard b){
  if((setMask(sq) & b) == 0x0ULL){
    return 0;
  }else{
    return 1;
  }
}



template <class Container>
void splitString(const std::string& str, const Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}


constexpr PieceType typeOfPiece(const Piece p) {
  return PieceType(p & 7);
}

constexpr Color colorOfPiece(const Piece p) {
  assert(p != NO_PIECE);
  return Color(p >> 3);
}

constexpr Piece pieceOfTypeColor(const PieceType n, const Color c) {
  assert(p != NO_PIECE);
  return c == WHITE ? Piece(n) : Piece(n + 8) ;
}

inline Color charToPieceColor(const char p){
  if(isupper(p)){
    return WHITE;
  }else if (islower(p)){
    return BLACK;
  }else{
    return COLOR_INIT;
  }
}


inline char pieceToChar(const Piece p){

    return asciiPieceString[p];

}

inline Piece charToPiece(const char p){
  

    for(int i=0;i< int(asciiPieceString.length());i++){

      if(asciiPieceString[i] == p){
        return Piece(i);
      }
    }
  return NO_PIECE;
}


inline PieceType charToPieceType(const char p){
  
  return typeOfPiece(charToPiece(p));

}

inline std::string squareToString(const Square s){
  if(s == SQUARE_NONE)
    return "-";
  return std::string{ char('a' + file_of(s)), char('1' + rank_of(s)) };
}

inline void printMove(const Move m){

    using namespace std;

    cout << "Source square: " << squareToString(decodeMove<SOURCE_SQUARE>(m)) << endl;
    cout << "Target square: " << squareToString(decodeMove<TARGET_SQUARE>(m)) << endl;
    cout << "Piece: " << pieceToChar(decodeMove<PIECE>(m)) << endl;
    cout << "Promoted Piece: " << pieceToChar(decodeMove<PROMOTED_PIECE>(m)) << endl;
    cout << "Is Capture: " << decodeMove<CAPTURE>(m) << endl;
    cout << "Is Double push: " << decodeMove<DOUBLE_PUSH>(m) << endl;
    cout << "Is Enpassant: " << decodeMove<ENPASSANT>(m) << endl;
    cout << "Is Castling: " << decodeMove<CASTLING>(m) << endl;
}

  inline void printMoveList(const std::vector<Move> &MoveList){

    using namespace std;
    for(Move m : MoveList){
        cout << " ------------------MOVE----------------- " << endl;
        printMove(m);
    }
    cout << " --------------------------------------- " << endl;
}


  // trim from start (in place)
  static inline void ltrim(std::string& s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
          return !std::isspace(ch);
          }));
  }

  // trim from end (in place)
  static inline void rtrim(std::string& s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
          return !std::isspace(ch);
          }).base(), s.end());
  }

  // trim from both ends (in place)
  static inline void trim(std::string& s) {
      ltrim(s);
      rtrim(s);
  }

  // trim from both ends (copying)
  static inline std::string trim_copy(std::string s) {
      trim(s);
      return s;
  }

inline void pretyBB(Bitboard b){

  std::stringstream os;

  for (int row = 7; row >= 0; --row) {
      for (int col = 0; col <= 7; ++col) {
        if (b & (1ULL << ((row * 8) + col))) {
          os << "1 ";
        } else {
          os << ". ";
        }
      }
      os << "\n";
    }

    std::cout << os.str() << std::endl;
}
