#ifndef TESTING
#define TESTING

#include <stdio.h>
#include <time.h>
#include "moves.h"
#include "chessboard.h"
#include "search.h"

unsigned long long perft(Chessboard* board, int depth);
unsigned long long perft_test(Chessboard* board, int depth);


/*
    Here perft results for several positions that are useful for debugging.
    These are taken from the chess programming wiki.
    link: https://www.chessprogramming.org/Perft_Results
*/

#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define pos1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define pos2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "
#define pos3 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "
#define pos4 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define pos5 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  "
#define pos6 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"



void game(int depth);
int in_checkmate(Chessboard* board);


struct Test_case{
    char* position;
    unsigned long long perft_results[6];    // perft results for depth 1 to 7
};

void search_speed(int depth);
void test_move_generator();
#endif