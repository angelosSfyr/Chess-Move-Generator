#ifndef MAGIC
#define MAGIC

#include <stdio.h>
#include <stdint.h>
#include "bitboard.h"


// bishop relevant occupancy bit count for every square on board
extern uint64_t bishop_magic_numbers[64];
extern uint64_t rook_magic_numbers[64] ;
extern int bishop_relevant_bits[64] ;
extern int rook_relevant_bits[64];

unsigned int pseudo32random();
uint64_t pseudo64random();
uint64_t magic_number();
uint64_t find_magic_number(int square, int relevant_bits, int bishop);
void init_magic_numbers();


inline int magic_index(int magic_number, bitboard occ, int rel_bits)
{
    return (occ*magic_number) >> (64 - rel_bits);
}

#endif  