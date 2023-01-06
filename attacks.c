#include <stdio.h>
#include "bitboard.h"
#include "attacks.h"
#include "magic.h"
#include "chessboard.h"



bitboard pawn_attack_mask(int bit_index, int side)
{ /* from the enumeration WHITE = 0, BLACK = 1 */

    bitboard pawn = 0ULL;
    bitboard attack_mask = 0ULL;
    set_bit(pawn, bit_index);

    if (side == white)
    {
        /* white side */
        if (pawn & not_A_file)
            attack_mask |= (pawn << 9); /* left up diagonal attack */
        if (pawn & not_H_file)
            attack_mask |= (pawn << 7); /* right up diagonal attack */
    }
    else
    {
        /* black side */
        if (pawn & not_A_file)
            attack_mask |= (pawn >> 7); /* left down diagonal attack */
        if (pawn & not_H_file)
            attack_mask |= (pawn >> 9); /* right down diagonal attack */
    }
    return attack_mask;
}


bitboard knight_attack_mask(int bit_index)
{

    bitboard knight = 0ULL;
    bitboard attack_mask = 0ULL;
    set_bit(knight, bit_index);

    /* left attacks */
    if (knight & not_A_file)
        attack_mask |= (knight << 17);
    if (knight & not_AB_file)
        attack_mask |= (knight << 10);
    if (knight & not_A_file)
        attack_mask |= (knight >> 15);
    if (knight & not_AB_file)
        attack_mask |= (knight >> 6);
    /* right attacks */
    if (knight & not_H_file)
        attack_mask |= (knight << 15);
    if (knight & not_HG_file)
        attack_mask |= (knight << 6);
    if (knight & not_H_file)
        attack_mask |= (knight >> 17);
    if (knight & not_HG_file)
        attack_mask |= (knight >> 10);

    return attack_mask;
}


bitboard King_attack_mask(int bit_index)
{
    bitboard King = 0ULL;
    bitboard attack_mask = 0ULL;
    set_bit(King, bit_index);
    /*  clockwise check --> left up right down*/
    if (King & not_A_file)      attack_mask |= (King << 1) | (King << 9) | (King >> 7);
    if (King & not_H_file)      attack_mask |= (King >> 1) | (King << 7) | (King >> 9);
    attack_mask |= (King << 8) | (King >> 8);
    return attack_mask;
}


bitboard rook_attack_mask(int bit_index)
{
    int i, j;
    int cur_rank, cur_file;
    bitboard rook = 0ULL;
    bitboard attack_mask = 0ULL;
    set_bit(rook, bit_index);
    cur_rank = bit_index / 8;
    cur_file = bit_index % 8;
    /* right */
    for (j = cur_file + 1; j < 7; j++) set_bit(attack_mask, 8 * cur_rank + j); 
    /* left */
    for (j = cur_file - 1; j > 0; j--) set_bit(attack_mask, 8 * cur_rank + j); 
    /* up */
    for (i = cur_rank - 1; i > 0; i--) set_bit(attack_mask, i * 8 + cur_file); 
    /* up */
    for (i = cur_rank + 1; i < 7; i++) set_bit(attack_mask, i * 8 + cur_file); 

    return attack_mask;
}


bitboard rook_attack_blocked_mask(int bit_index, bitboard blocker_board)
{
    int i, j;
    int cur_rank, cur_file;
    bitboard rook = 0ULL;
    bitboard attack_mask = 0ULL;
    set_bit(rook, bit_index);
    cur_rank = bit_index / 8;
    cur_file = bit_index % 8;
    //left
    for (j = cur_file + 1; j <= 7; j++)
    {
        set_bit(attack_mask, 8 * cur_rank + j); 
        if ((1ULL << ((8 * cur_rank + j))) & blocker_board)   break;
    }
    // right 
    for (j = cur_file - 1; j >= 0; j--)
    {
        set_bit(attack_mask, 8 * cur_rank + j); 
        if ((1ULL << ((8 * cur_rank + j))) & blocker_board)   break;
    }
    // down 
    for (i = cur_rank - 1; i >= 0; i--)
    {
        set_bit(attack_mask, i * 8 + cur_file);
        if ((1ULL << ((8 * i + cur_file))) & blocker_board)   break;
    }
    // up 
    for (i = cur_rank + 1; i <= 7; i++)
    {
        set_bit(attack_mask, i * 8 + cur_file); 
        if ((1ULL << ((8 * i + cur_file))) & blocker_board)   break;
    }
    return attack_mask;
}


bitboard bishop_attack_mask(int bit_index)
{
    int i, j;
    int cur_rank, cur_file;
    bitboard bishop = 0ULL;
    bitboard attack_mask = 0ULL;
    set_bit(bishop, bit_index);
    /* right up left shift by 7 */
    cur_rank = bit_index / 8;
    cur_file = bit_index % 8;
    /* up */
    for (i = (cur_rank - 1), j = (cur_file - 1); (i > 0) && (j > 0); i--, j--)
        set_bit(attack_mask, i * 8 + j);
    for (i = (cur_rank - 1), j = (cur_file + 1); (i > 0) && (j < 7); i--, j++)
        set_bit(attack_mask, i * 8 + j);
    for (i = (cur_rank + 1), j = (cur_file - 1); (i < 7) && (j > 0); i++, j--)
        set_bit(attack_mask, i * 8 + j);
    for (i = (cur_rank + 1), j = (cur_file + 1); (i < 7) && (j < 7); i++, j++)
        set_bit(attack_mask, i * 8 + j);
    return attack_mask;
}


bitboard bishop_attack_blocked_mask(int bit_index, bitboard blocker_board)
{
    int i, j;
    int cur_rank, cur_file;
    bitboard bishop = 0ULL;
    bitboard attack_mask = 0ULL;
    set_bit(bishop, bit_index);
    /* right up left shift by 7 */
    cur_rank = bit_index / 8;
    cur_file = bit_index % 8;
    /* up */
    for (i = (cur_rank - 1), j = (cur_file - 1); (i >= 0) && (j >= 0); i--, j--)
    {
        set_bit(attack_mask, i * 8 + j);
        if ((1ULL << (i * 8 + j)) & blocker_board)   break;
    }
    for (i = (cur_rank - 1), j = (cur_file + 1); (i >= 0) && (j <= 7); i--, j++)
    {
        set_bit(attack_mask, i * 8 + j);
        if ((1ULL << (i * 8 + j)) & blocker_board)   break;
    }
    for (i = (cur_rank + 1), j = (cur_file - 1); (i <= 7) && (j >= 0); i++, j--)
    {
        set_bit(attack_mask, i * 8 + j);
        if ((1ULL << (i * 8 + j)) & blocker_board)   break;
    }
    for (i = (cur_rank + 1), j = (cur_file + 1); (i <= 7) && (j <= 7); i++, j++)
    {
        set_bit(attack_mask, i * 8 + j);
        if ((1ULL << (i * 8 + j)) & blocker_board)   break;
    }
    return attack_mask;
}


bitboard set_occupancy(int index, int bits_in_mask, bitboard mask)
{
    bitboard occ = 0ULL;
    for (int count = 0; count < bits_in_mask; count++)
    {
        int square = LSB(mask);
        pop_bit(mask, square);
        if (index & (1 << count))
        {
            occ |= (1ULL << square);
        }
    }
    return occ;
}


void  init_pawn_attack_table()
{
    for (int square = 0; square < 64; square++)
    {
        pawn_attack_table[white][square] = 0ULL | pawn_attack_mask(square, white);
        pawn_attack_table[black][square] = 0ULL | pawn_attack_mask(square, black); 
    }
}

void init_knight_attack_table()
{ 
    for (int square = 0; square < 64; square ++)
    {
        knight_attack_table[square] = knight_attack_mask(square);
    }
}


void init_King_attack_table()
{
    for (int square = 0; square < 64; square ++){
        King_attack_table[square] = King_attack_mask(square);
    }   
}


void init_bishop_attack_masks()
{
    for (int square = 0; square <  64; square ++)
    {
        bishop_attack_masks[square] = bishop_attack_mask(square);
    } 
}


void init_rook_attack_masks()
{
    for (int square = 0; square < 64; square ++)
    {
        rook_attack_masks[square] = rook_attack_mask(square);
    } 
}


void init_bishop_attack_table()
{
    for (int square = 0; square < 64; square++)
    {   
            bitboard attack_mask = bishop_attack_masks[square];
            int  relevant_bits_count = bit_count(attack_mask);
            int occupancy_indices = (1 << relevant_bits_count); 
            for (int occ = 0; occ < occupancy_indices; occ++)
            { 
                // for every permutation  of blocker_board compute the magic index and initialize the attack table 
                bitboard occupancy = set_occupancy(occ, relevant_bits_count, attack_mask);    
                int mag_index = (occupancy*bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);
                bishop_attack_table[square][mag_index] = bishop_attack_blocked_mask(square, occupancy);
            }
    }
}


void init_rook_attack_table()
{
    for (int square = 0; square < 64; square++)
    {
        bitboard attack_mask = rook_attack_masks[square];
        int relevant_bits_count = bit_count(attack_mask);
        int occupancy_indices = (1 << relevant_bits_count);
        for (int occ = 0; occ < occupancy_indices; occ++)
        {
            // for every permutation  of blocker_board get the magic index and initialize the attack table
            bitboard occupancy = set_occupancy(occ, relevant_bits_count, attack_mask);
            int mag_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
            rook_attack_table[square][mag_index] = rook_attack_blocked_mask(square, occupancy);
        }
    }
}

bitboard get_pawn_attack(int side, int square)
{
    return pawn_attack_table[side][square];
}

bitboard get_knight_attack(int square)
{
    return knight_attack_table[square];
}

bitboard get_bishop_attack(int square, bitboard occupancy)
{
    occupancy &= bishop_attack_masks[square];
    occupancy *= bishop_magic_numbers[square];   
    occupancy >>= (64 - bishop_relevant_bits[square]);
    return bishop_attack_table[square][occupancy];
}


bitboard get_rook_attack(int square, bitboard occupancy)
{
   occupancy &= rook_attack_masks[square];
   occupancy *= rook_magic_numbers[square];   
   occupancy >>= (64 - rook_relevant_bits[square]);
   return rook_attack_table[square][occupancy];
}


bitboard get_queen_attack(int square, bitboard occupancy)
{
    return get_bishop_attack(square, occupancy) | get_rook_attack(square, occupancy);
}


bitboard get_king_attack(int square)
{
    return King_attack_table[square];
}


void initialize_attacks()
{
    init_rook_attack_masks();
    init_rook_attack_table();
    init_bishop_attack_masks();
    init_bishop_attack_table();
    init_pawn_attack_table();
    init_knight_attack_table();
    init_King_attack_table();
}