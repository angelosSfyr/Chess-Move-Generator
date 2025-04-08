#include <stdio.h>
#include <stdlib.h>
#include "chessboard.h"
#include "moves.h"
#include "testing.h"
#include "search.h"
#include "bitboard.h"


unsigned long long perft(Chessboard* board, int depth)
{
    unsigned long long count = 0;
    if (depth == 0 )    return 1;
    Moves  moves;
    Chessboard cp;
    copy_chessboard(&cp, board);
    generate_moves(board, &moves);
    for (int i=0; i < moves.number_of_moves; i++)
    {        
        if (make_move(board, moves.moves[i]))
        {
            count +=  perft(board, depth-1);
        }
        copy_chessboard(board, &cp);
    }
    return count;
}


unsigned long long perft_test(Chessboard* board, int depth)
{
    unsigned long long count = 0;
    printf("\n     Performance test\n\n");
    Moves moves;
    Chessboard copy;
    copy_chessboard(&copy, board);
    generate_moves(board, &moves);
    //time_t start = clock();   add time measurements
    for (int move=0; move < moves.number_of_moves; move++)
    {   
        if (!make_move(board, moves.moves[move]))
        {
            continue;
        } 
        long nodes = perft(board, depth-1);
        printf("Move: %s%s \t Nodes reached: %ld\n", square_to_coordinates[get_move_source(moves.moves[move])],
                                                square_to_coordinates[get_move_target(moves.moves[move])], 
                                                nodes);
        count += nodes;
        copy_chessboard(board, &copy);
    }
    return count;
}


const struct Test_case case1 = {pos1, {20, 400, 8902, 197281, 4865609, 119060324}};
const struct Test_case case2 = {pos2, {48, 2039, 97862, 4085603, 193690690, 8031647685}};
const struct Test_case case3 = {pos3, {14, 191, 2812, 43238, 674624, 11030083}};
const struct Test_case case4 = {pos4, {6, 264, 9467, 422333, 15833292, 706045033}};
const struct Test_case case5 = {pos5, {44, 1486, 62379, 2103487, 89941194,  3048196529}};
const struct Test_case case6 = {pos6, {46, 2079, 89890, 3894594, 164075551,  6923051137}};


void test_move_generator()
{
    initialize_attacks();
    Chessboard board;
    struct Test_case cases[6] = {case1, case2, case3, case4, case5, case6};
    for (int i=0; i<6; i++)
    {
        printf("----------------CASE: %d----------------\n", i+1);
        parse_fen(&board, cases[i].position);
        printf("(PROGRAM RESULTS)\n");
        for (int d=1; d < 6; d++)
        {
            printf("%lld ", perft(&board, d));
        }
        printf("\n(CORRECT RESULTS)\n");
        for (int d=0; d < 5; d++)
        {
            printf("%lld ", cases[i].perft_results[d]);
        }
        printf("\n");
    }
}