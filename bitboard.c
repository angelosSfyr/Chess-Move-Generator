#include "bitboard.h"


void print_bb(bitboard bb)
{
    for (int i = 7; i >=0; i--)
    {
        printf("%d  ", i+1);
        for (int j = 7; j >= 0; j--)
        {
            int sq_bit = (i*8)+j;
            printf("%d ", get_bit(bb, sq_bit)?1:0);
        }
        printf("\n");
    }
    printf("\n   A B C D E F G H\n");
}


const int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

/**
 * bitScanForward
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */


int LSB(bitboard bb) 
{
   const bitboard debruijn64 = (bitboard)(0x03f79d71b4cb0a89);
   return index64[((bb & -bb) * debruijn64) >> 58];
}

int bit_count(bitboard x)
{
    int count = 0;
    while(x)
    {
        count += x & 1ULL;
        x = x >> 1;
    } 
    return count;
}