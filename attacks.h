#ifndef ATTACKS
#define ATTACKS


#include <stdint.h>
#include "bitboard.h"
#include "chessboard.h"



#define not_A_file   9187201950435737471ULL
#define not_H_file  18374403900871474942ULL
#define not_AB_file  4557430888798830399ULL
#define not_HG_file 18229723555195321596ULL



bitboard bishop_attack_masks[64];
bitboard rook_attack_masks[64];
bitboard bishop_attack_table[64][512];
bitboard rook_attack_table[64][4096];    
bitboard pawn_attack_table[2][64];
bitboard knight_attack_table[64];
bitboard King_attack_table[64];



bitboard rook_attack_mask(int square);
bitboard rook_attack_blocked_mask(int square, bitboard blocks);

bitboard bishop_attack_mask(int square);
bitboard bishop_attack_blocked_mask(int square, bitboard blocks);


bitboard pawn_attack_mask(int bit_index, int side);
bitboard knight_attack_mask(int bit_index);
bitboard King_attack_mask(int bit_index);

bitboard rook_attack_mask(int square);
bitboard rook_attack_blocked_mask(int square, bitboard blocks);

bitboard bishop_attack_mask(int square);
bitboard bishop_attack_blocked_mask(int square, bitboard blocks);

bitboard set_occupancy(int index, int bits_in_mask, bitboard mask);

bitboard get_pawn_attack(int square, int side);
bitboard get_knight_attack(int square);
bitboard get_bishop_attack(int square, bitboard occupancy);
bitboard get_rook_attack(int square, bitboard occupancy);
bitboard get_queen_attack(int square, bitboard occupancy);
bitboard get_king_attack(int square);
void initialize_attacks();



#endif