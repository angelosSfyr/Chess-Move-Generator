#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "magic.h"
#include "attacks.h"
#include "bitboard.h"


int bishop_relevant_bits[64] = 
{
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board
int rook_relevant_bits[64] = 
{
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};



unsigned int pseudo32random()
{
    static unsigned int state = 1804289383;
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}


uint64_t pseudo64random()
{
    uint64_t u1, u2, u3, u4;
    u1 = (uint64_t)(pseudo32random()) & 0xFFFF;
    u2 = (uint64_t)(pseudo32random()) & 0xFFFF;
    u3 = (uint64_t)(pseudo32random()) & 0xFFFF;
    u4 = (uint64_t)(pseudo32random()) & 0xFFFF;
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}


uint64_t magic_number()
{
    return pseudo64random() & pseudo64random() & pseudo64random();
}


uint64_t bishop_magic_numbers[64] = 
{
    0x40040822862081ULL, 
    0x40810a4108000ULL, 
    0x2008008400920040ULL, 
    0x61050104000008ULL, 
    0x8282021010016100ULL, 
    0x41008210400a0001ULL, 
    0x3004202104050c0ULL, 
    0x22010108410402ULL, 
    0x60400862888605ULL, 
    0x6311401040228ULL, 
    0x80801082000ULL, 
    0x802a082080240100ULL, 
    0x1860061210016800ULL, 
    0x401016010a810ULL, 
    0x1000060545201005ULL, 
    0x21000c2098280819ULL, 
    0x2020004242020200ULL, 
    0x4102100490040101ULL, 
    0x114012208001500ULL, 
    0x108000682004460ULL, 
    0x7809000490401000ULL, 
    0x420b001601052912ULL, 
    0x408c8206100300ULL, 
    0x2231001041180110ULL, 
    0x8010102008a02100ULL, 
    0x204201004080084ULL, 
    0x410500058008811ULL, 
    0x480a040008010820ULL, 
    0x2194082044002002ULL, 
    0x2008a20001004200ULL, 
    0x40908041041004ULL, 
    0x881002200540404ULL, 
    0x4001082002082101ULL, 
    0x8110408880880ULL, 
    0x8000404040080200ULL, 
    0x200020082180080ULL, 
    0x1184440400114100ULL, 
    0xc220008020110412ULL, 
    0x4088084040090100ULL, 
    0x8822104100121080ULL, 
    0x100111884008200aULL, 
    0x2844040288820200ULL, 
    0x90901088003010ULL, 
    0x1000a218000400ULL, 
    0x1102010420204ULL, 
    0x8414a3483000200ULL, 
    0x6410849901420400ULL, 
    0x201080200901040ULL, 
    0x204880808050002ULL, 
    0x1001008201210000ULL, 
    0x16a6300a890040aULL, 
    0x8049000441108600ULL, 
    0x2212002060410044ULL, 
    0x100086308020020ULL, 
    0x484241408020421ULL, 
    0x105084028429c085ULL, 
    0x4282480801080cULL, 
    0x81c098488088240ULL, 
    0x1400000090480820ULL, 
    0x4444000030208810ULL, 
    0x1020142010820200ULL, 
    0x2234802004018200ULL, 
    0xc2040450820a00ULL, 
    0x2101021090020ULL
};


uint64_t rook_magic_numbers[64] = 
{
    0xa080041440042080ULL, 
    0xa840200410004001ULL, 
    0xc800c1000200081ULL, 
    0x100081001000420ULL, 
    0x200020010080420ULL, 
    0x3001c0002010008ULL, 
    0x8480008002000100ULL, 
    0x2080088004402900ULL, 
    0x800098204000ULL, 
    0x2024401000200040ULL, 
    0x100802000801000ULL, 
    0x120800800801000ULL, 
    0x208808088000400ULL, 
    0x2802200800400ULL, 
    0x2200800100020080ULL, 
    0x801000060821100ULL, 
    0x80044006422000ULL, 
    0x100808020004000ULL, 
    0x12108a0010204200ULL, 
    0x140848010000802ULL, 
    0x481828014002800ULL, 
    0x8094004002004100ULL, 
    0x4010040010010802ULL, 
    0x20008806104ULL, 
    0x100400080208000ULL, 
    0x2040002120081000ULL, 
    0x21200680100081ULL, 
    0x20100080080080ULL, 
    0x2000a00200410ULL, 
    0x20080800400ULL, 
    0x80088400100102ULL, 
    0x80004600042881ULL, 
    0x4040008040800020ULL, 
    0x440003000200801ULL, 
    0x4200011004500ULL, 
    0x188020010100100ULL, 
    0x14800401802800ULL, 
    0x2080040080800200ULL, 
    0x124080204001001ULL, 
    0x200046502000484ULL, 
    0x480400080088020ULL, 
    0x1000422010034000ULL, 
    0x30200100110040ULL, 
    0x100021010009ULL, 
    0x2002080100110004ULL, 
    0x202008004008002ULL, 
    0x20020004010100ULL, 
    0x2048440040820001ULL, 
    0x101002200408200ULL, 
    0x40802000401080ULL, 
    0x4008142004410100ULL, 
    0x2060820c0120200ULL, 
    0x1001004080100ULL, 
    0x20c020080040080ULL, 
    0x2935610830022400ULL, 
    0x44440041009200ULL, 
    0x280001040802101ULL, 
    0x2100190040002085ULL, 
    0x80c0084100102001ULL, 
    0x4024081001000421ULL, 
    0x20030a0244872ULL, 
    0x12001008414402ULL, 
    0x2006104900a0804ULL, 
    0x1004081002402ULL
};



uint64_t find_magic(int square, int relevant_bits, int isBishop)
{
    /* Occupancies are all the possible permutations of block bitboard. */
    int occ_indicies = 1 << relevant_bits;  
    bitboard occupancies[4096];
    bitboard attacks[4096];
    bitboard used_attacks[4096];
    bitboard attack_mask = isBishop ? bishop_attack_mask(square):rook_attack_mask(square);
    /* Initialize occpancies and attacks */
    for (int i = 0; i < occ_indicies; i++){
        occupancies[i] = (set_occupancy(i, relevant_bits, attack_mask));
        attacks[i] = isBishop ? bishop_attack_blocked_mask(square, occupancies[i]):
                                rook_attack_blocked_mask(square, occupancies[i]);
    }
    /* find magic number with trial and error method */
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        /* magic numebr candidate */
        uint64_t magic = magic_number();    /* the magic number */
        /* indianess does not matter if you think about it */
        if (bit_count((attack_mask * magic) & 0xFF00000000000000) < 6) continue;
        // init used attacks
        for(int i =0; i < occ_indicies; i++) used_attacks[i] = 0ULL;
        int index, fail;
        // test magic index loop
        for ( index = 0, fail = 0; !fail && (index < occ_indicies); index++)
        {
            int magic_index = (int)((occupancies[index] * magic) >> (64 - relevant_bits));
            // if magic index works  initialize used attacks
            if (used_attacks[magic_index] == 0ULL)  used_attacks[magic_index] = attacks[index];
            // otherwise magic index doesn't work
            else if (used_attacks[magic_index] != attacks[index]) fail = 1;              
        }
        // if magic number works return it
        if (!fail) {
            return magic;
        }   
    }
    printf("  Magic number fails!\n");
    return 0ULL;
}


void init_magic_numbers()
{
    for(int square =0; square < 64; square++){
            // bishop 
            printf("0x%lxULL, \n", find_magic(square, bishop_relevant_bits[square], 1));
    }
    printf("\n");
    for(int square =0; square < 64; square++){
            // rook 
            printf("0x%lxULL, \n", find_magic(square, rook_relevant_bits[square], 0));
    }
}