#pragma once

#include "types.h"
#include "utils.h"
#include "board.h"

#define test_position "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
/*
1	20
2	400
3	8,902
4	197,281
5	4,865,609
6	119,060,324
7	3,195,901,860
8	84,998,978,956
9	2,439,530,234,167
*/
#define position_2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
/*
1	48
2	2039
3	97862
4	4085603
5	193690690
6	8031647685
*/
#define position_3 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
/*
Depth	Nodes
1	14
2	191
3	2812
4	43238
5	674624
6	11030083
7	178633661
8	3009794393
*/
#define position_4 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
/*
Depth	Nodes
1	6
2	264
3	9467
4	422333
5	15833292
6	706045033
*/
#define position_5 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
/*
Depth	Nodes
1	44
2	1,486
3	62,379
4	2,103,487
5	89,941,194
*/
#define position_6 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "
/*
Depth	Nodes
0	1
1	46
2	2,079
3	89,890
4	3,894,594
5	164,075,551
6	6,923,051,137
7	287,188,994,746
8	11,923,589,843,526
9	490,154,852,788,714
*/


typedef struct positionSignatureTag {
   const char *fen;
   int depth;
   uint64_t nodes;
} positionSignature;

static positionSignature perftests[] = {
   // Martin Sedlak's test positions
   // (http://www.talkchess.com/forum/viewtopic.php?t=47318)
   // avoid illegal ep
   { "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1",         6, 1134888 },
   { "8/8/8/8/k1p4R/8/3P4/3K4 w - - 0 1",         6, 1134888 },
   { "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1",         6, 1015133 },
   { "8/b2p2k1/8/2P5/8/4K3/8/8 b - - 0 1",         6, 1015133 },
   // en passant capture checks opponent: 
   { "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1",         6, 1440467 },
   { "8/5k2/8/2Pp4/2B5/1K6/8/8 w - d6 0 1",         6, 1440467 },
   // short castling gives check: 
   { "5k2/8/8/8/8/8/8/4K2R w K - 0 1",            6, 661072 },
   { "4k2r/8/8/8/8/8/8/5K2 b k - 0 1",            6, 661072 },
   // long castling gives check: 
   { "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1",            6, 803711 },
   { "r3k3/8/8/8/8/8/8/3K4 b q - 0 1",            6, 803711 },
   // castling (including losing cr due to rook capture): 
   { "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1",   4, 1274206 },
   { "r3k2r/7b/8/8/8/8/1B4BQ/R3K2R b KQkq - 0 1",    4, 1274206 },
   // castling prevented: 
   { "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1",   4, 1720476 },
   { "r3k2r/8/5Q2/8/8/3q4/8/R3K2R w KQkq - 0 1",   4, 1720476 },
   // promote out of check: 
   { "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1",         6, 3821001 },
   { "3K4/8/8/8/8/8/4p3/2k2R2 b - - 0 1",         6, 3821001 },
   // discovered check: 
   { "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1",         5, 1004658 },
   { "5K2/8/1Q6/2N5/8/1p2k3/8/8 w - - 0 1",         5, 1004658 },
   // promote to give check: 
   { "4k3/1P6/8/8/8/8/K7/8 w - - 0 1",            6, 217342 },
   { "8/k7/8/8/8/8/1p6/4K3 b - - 0 1",            6, 217342 },
   // underpromote to check: 
   { "8/P1k5/K7/8/8/8/8/8 w - - 0 1",            6, 92683 },
   { "8/8/8/8/8/k7/p1K5/8 b - - 0 1",            6, 92683 },
   // self stalemate: 
   { "K1k5/8/P7/8/8/8/8/8 w - - 0 1",            6, 2217 },
   { "8/8/8/8/8/p7/8/k1K5 b - - 0 1",            6, 2217 },
   // stalemate/checkmate: 
   { "8/k1P5/8/1K6/8/8/8/8 w - - 0 1",            7, 567584 },
   { "8/8/8/8/1k6/8/K1p5/8 b - - 0 1",            7, 567584 },
   // double check: 
   { "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1",         4, 23527 },
   { "8/5k2/8/5N2/5Q2/2K5/8/8 w - - 0 1",         4, 23527 },

   // short castling impossible although the rook never moved away from its corner 
   { "1k6/1b6/8/8/7R/8/8/4K2R b K - 0 1", 5, 1063513 },
   { "4k2r/8/8/7r/8/8/1B6/1K6 w k - 0 1", 5, 1063513 },

   // long castling impossible although the rook never moved away from its corner 
   { "1k6/8/8/8/R7/1n6/8/R3K3 b Q - 0 1", 5, 346695 },
   { "r3k3/8/1N6/r7/8/8/8/1K6 w q - 0 1", 5, 346695 },

   // From the Wiki
   { "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 4, 4085603 },
   { "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6", 3, 53392 },

   // Shortened form of the third position below
   { "8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28", 4, 67197 },

   // John Merlino's test positions, some of these take a long time, only do them
   // in debug mode.
#ifdef DEBUGMODE
   { "r3k2r/8/8/8/3pPp2/8/8/R3K1RR b KQkq e3 0 1", 6, 485647607 },
   { "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6, 706045033 },
   { "8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28", 6, 38633283 },
   { "8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - - 1 67", 7, 493407574 },
   { "rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3", 6, 244063299 },
   { "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 5, 193690690 },
   { "8/p7/8/1P6/K1k3p1/6P1/7P/8 w - -", 8, 8103790 },
   { "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - -", 6, 71179139 },
   { "r3k2r/p6p/8/B7/1pp1p3/3b4/P6P/R3K2R w KQkq -", 6, 77054993 },

   { "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 7, 178633661 },
   { "8/5p2/8/2k3P1/p3K3/8/1P6/8 b - -", 8, 64451405 },
   { "r3k2r/pb3p2/5npp/n2p4/1p1PPB2/6P1/P2N1PBP/R3K2R w KQkq -", 5, 29179893 },
#endif

   { NULL, 0, 0 }
};


uint64_t perftDriver(Board &board, int depth);

uint64_t perftTest(int depth, std::string fen_string, bool divide = false);

void perftTestDebug();