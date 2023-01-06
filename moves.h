#ifndef MOVES
#define MOVES

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "bitboard.h"
#include "attacks.h"
#include "chessboard.h"



enum { wk = 1,   wq = 2,      bk = 4,     bq = 8 };
/* 0001    1  white king can castle to the king side
   0010    2  white king can castle to the queen side
   0100    4  black king can castle to the king side
   1000    8  black king can castle to the queen side */ 
/* pieces encoding */


#define encode_move(ss, ts, piece, prom_piece, capt_fl, dpush_fl, enp_fl, castl_fl) \
    (ss | (ts << 6) | (piece << 12) | (prom_piece << 16) | (capt_fl << 20) | (dpush_fl << 21) | (enp_fl << 22) | (castl_fl << 23))

#define get_move_source(move) (move & 0x3f)

#define get_move_target(move) ((move & 0xfc0) >> 6)

#define get_move_piece(move) ((move & 0xf000) >> 12)

#define get_move_promoted(move) ((move & 0xf0000) >> 16)

#define get_move_capture(move) (move & 0x100000)

#define get_move_double(move) (move & 0x200000)

#define get_move_enpassant(move) (move & 0x400000)

#define get_move_castling(move) (move & 0x800000)


typedef struct 
{
    int moves[256];
    int number_of_moves;
} Moves;


void castle_moves(Chessboard *board, Moves *moves);
void add_move(Moves* moves, int move);
void print_move(int move);
void print_moves(Moves* moves);
void pawn_moves(Chessboard* board, Moves* moves);
int generate_moves(Chessboard* board, Moves* moves);
int make_move(Chessboard* board, int move);

#endif
