#ifndef BITBOARD
#define BITBOARD


#include <stdio.h>
#include <stdint.h>


enum {white, black, white_and_black};
enum { P, N, B, R, Q, K, p, n, b, r, q, k};

typedef uint64_t bitboard;

/* 
    This is the square mapping.
    a8 is the last bit(msb) ->  index = 63
    h1 is the first bit(lsb) -> index = 0
*/


enum 
{
    a8=63, b8=62, c8=61, d8=60, e8=59, f8=58, g8=57, h8=56,
    a7=55, b7=54, c7=53, d7=52, e7=51, f7=50, g7=49, h7=48,
    a6=47, b6=46, c6=45, d6=44, e6=43, f6=42, g6=41, h6=40,
    a5=39, b5=38, c5=37, d5=36, e5=35, f5=34, g5=33, h5=32,
    a4=31, b4=30, c4=29, d4=28, e4=27, f4=26, g4=25, h4=24,
    a3=23, b3=22, c3=21, d3=20, e3=19, f3=18, g3=17, h3=16,
    a2=15, b2=14, c2=13, d2=12, e2=11, f2=10, g2= 9, h2= 8,
    a1= 7, b1= 6, c1= 5, d1= 4, e1= 3, f1= 2, g1= 1, h1= 0, 
    no_square=64
}; 



#define set_bit(bb, bit) ((bb) |= ((1ULL) << (bit)))
#define pop_bit(bb, bit) ((bb) ^= ((1ULL) << (bit)))
#define get_bit(bb, bit) (((1ULL) << (bit)) & (bb))
#define pop_lsb(bb) ((bb) &= (bb-1))


void print_bb(bitboard bb);

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

// initial function name: bitScanForward
int LSB(bitboard bb);
int bit_count(bitboard x);

#endif