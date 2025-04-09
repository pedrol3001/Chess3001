#pragma once

#define DEBUG


#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <cstring>

#include <iostream>

// Bitboard typedef
typedef uint64_t Bitboard;


typedef uint32_t Move;

const Move NO_MOVE = 0UL;

const int64_t INF = 1000000;

extern uint64_t debugCounter;

// Chess Colors
enum Color{
    WHITE,
    BLACK,
    COLOR_INIT = 2
};

// Chess Squares
enum Square : int {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  SQUARE_NONE = -1,

  SQUARE_ZERO = 0,
  SQUARE_INIT   = 64
};

// Chess Piece Types
enum PieceType {
  ALL_PIECES = 0,
  PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
  NO_PIECE_TYPE = 7,
  PIECE_TYPE_INIT = 8
};


// Chess Idividual Pieces
enum Piece {

  NO_PIECE = 0,
  P = static_cast<int>(PieceType::PAWN), N, B, R, Q, K,
 // to facilitate calculus of string this NULL_PIECE offeset is used

  p = static_cast<int>(PieceType::PAWN) + 8, n, b, r, q, k,
  PIECE_INIT = 15,

};

const std::string asciiPieceString = "-PNBRQK--pnbrqk-";
//const std::wstring unicodeChessPiece = "--♟︎♞♝♜♛♚--♙♘♗♖♕♔--";


const wchar_t unicodeChessPiece[18] = {'-',L'\u265F',L'\u265E',L'\u265D',L'\u265C',L'\u265B',L'\u265A','-',
                               '-',L'\u2659',L'\u2658',L'\u2657',L'\u2656',L'\u2655',L'\u2654','-'};



enum CastlingRights {
  NO_CASTLING,
  WHITE_OO = 1,
  WHITE_OOO = 2,
  BLACK_OO  = 4,
  BLACK_OOO = 8,

  CASTLING_NB = 16
};

// castling rights update constants
const int castlingRightsConstants[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    7 , 15, 15, 15, 3 , 15, 15, 11
};


// Shift Directions in bitboards
enum Direction : int {
  NORTH =  8,
  EAST  =  1,
  SOUTH = -NORTH,
  WEST  = -EAST,

  NORTH_EAST = NORTH + EAST,
  SOUTH_EAST = SOUTH + EAST,
  SOUTH_WEST = SOUTH + WEST,
  NORTH_WEST = NORTH + WEST


};


enum MoveEncode : Move {
  SOURCE_SQUARE = 0x3f, // 6 bit square
  TARGET_SQUARE = 0xfc0, // 6 bit square
  PIECE = 0xf000, // 4 bit piece
  PROMOTED_PIECE  = 0xf0000, // 4 bit promoted piece
  CAPTURE = 0x100000, // 1 bit capture
  DOUBLE_PUSH = 0x200000, // 1 bit doublepush
  ENPASSANT = 0x400000, // 1 bit enpassant
  CASTLING = 0x800000, // 1 bit castling
};

enum MoveType {
  CAPTURE_MOVES,
  QUIET_MOVES,
  CASTLING_MOVES,
  ALL_MOVES,
  MOVE_TYPE_INIT
};

// Chess Files 
enum File : int {
  FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_INIT
};

// Chess Ranks
enum Rank : int {
  RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_INIT
};

#define ENABLE_BASE_OPERATORS_ON(T)                                \
constexpr T operator+(T d1, int d2) { return T(int(d1) + d2); }    \
constexpr T operator+(T d) { return T(+int(d)); }                  \
constexpr T operator-(T d1, int d2) { return T(int(d1) - d2); }    \
constexpr T operator-(T d) { return T(-int(d)); }                  \
inline T& operator+=(T& d1, int d2) { return d1 = d1 + d2; }       \
inline T& operator-=(T& d1, int d2) { return d1 = d1 - d2; }

#define ENABLE_INCR_OPERATORS_ON(T)                                 \
inline T& operator++(T& d) { return d = T(int(d) + 1); }            \
inline T& operator++(T& s,int) { return s = T(int(s) + 1); }        \
inline T& operator--(T& d) { return d = T(int(d) - 1); }            \
inline T& operator--(T& s,int) { return s = T(int(s) - 1); }

#define ENABLE_BITWISE_OPERATORS_ON(T)                                       \
constexpr T operator&(int i, T d) { return T(i & int(d)); }                  \
constexpr T operator|(int i, T d) { return T(i | int(d)); }                  \
inline T& operator|=(T& c1, T c2)   { return c1 =  T(int(c1) | int(c2)); }   \
inline T& operator|=(T& c1, int c2) { return c1 =  T(int(c1) | int(c2)); }   \
inline T& operator&=(T& c1, T c2)   { return c1 =  T(int(c1) & int(c2)); }   \
inline T& operator&=(T& c1, int c2) { return c1 =  T(int(c1) & int(c2)); }

#define ENABLE_FULL_OPERATORS_ON(T)                                \
ENABLE_BITWISE_OPERATORS_ON(T)                                     \
ENABLE_BASE_OPERATORS_ON(T)                                        \
constexpr T operator*(int i, T d) { return T(i * int(d)); }        \
constexpr T operator*(T d, int i) { return T(int(d) * i); }        \
constexpr T operator/(T d, int i) { return T(int(d) / i); }        \
constexpr int operator/(T d1, T d2) { return int(d1) / int(d2); }  \
inline T& operator*=(T& d, int i) { return d = T(int(d) * i); }    \
inline T& operator/=(T& d, int i) { return d = T(int(d) / i); }


ENABLE_BITWISE_OPERATORS_ON(CastlingRights)

ENABLE_FULL_OPERATORS_ON(Direction)

ENABLE_INCR_OPERATORS_ON(Piece)
ENABLE_INCR_OPERATORS_ON(PieceType)
ENABLE_INCR_OPERATORS_ON(Square)
ENABLE_INCR_OPERATORS_ON(File)
ENABLE_INCR_OPERATORS_ON(Rank)

#undef ENABLE_FULL_OPERATORS_ON
#undef ENABLE_INCR_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON
#undef ENABLE_BITWISE_OPERATORS_ON


/// Additional operators to add a Direction to a Square
constexpr Square operator+(Square s, Direction d) { return Square(int(s) + int(d)); }
constexpr Square operator-(Square s, Direction d) { return Square(int(s) - int(d)); }
constexpr Color operator!(Color s) { return (s == Color::WHITE ? Color::BLACK : Color::WHITE); }
inline Square& operator+=(Square& s, Direction d) { return s = s + d; }
inline Square& operator-=(Square& s, Direction d) { return s = s - d; }

