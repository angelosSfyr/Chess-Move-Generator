#ifndef CHESSBOARD
#define CHESSBOARD


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "bitboard.h"
#include "attacks.h"



extern const char* square_to_coordinates[64];
extern char* uni_pieces[12];

typedef struct
{
    bitboard bitboard[12]; /* 6 bitboards for white,  6 bitboards for black */
    bitboard occupancy[3];
    int castling_rights;  
    int side;   // initlize -1
    int enpassant;  // initialize no square
} Chessboard;


void parse_fen(Chessboard* board, char const *fen);
void print_uni_board(Chessboard* board);
void reset_occupancies(Chessboard* board);
int is_square_attacked(Chessboard* board, int square, int side);
void copy_chessboard(Chessboard* to, Chessboard* from);

int get_side(Chessboard* board);
int get_enpassant(Chessboard* board);
int get_castling_rights(Chessboard* board);
bitboard get_piece_bb(Chessboard* board, int piece);
bitboard get_occupancy_bb(Chessboard* board, int side);

#endif