#include "moves.h"
#include "chessboard.h"
#include "bitboard.h"
#include <time.h>



void add_move(Moves *moves, int move)
{
    moves->moves[moves->number_of_moves] = move;
    moves->number_of_moves++;
}


void print_move(int move)
{
    printf("%s ~ ", uni_pieces[get_move_piece(move)]);
    printf("%s -> %s", square_to_coordinates[get_move_source(move)], square_to_coordinates[get_move_target(move)]);
    if (get_move_capture(move))
        printf("[Capture]");
    if (get_move_castling(move))
        printf("[Castle]");
    if (get_move_double(move))
        printf("[Double]");
    if (get_move_enpassant(move))
        printf("[Enpassant]");
    if (get_move_promoted(move))
        printf("[Promo->  %s ]", uni_pieces[get_move_promoted(move)]);
    // insert here
    printf(" \n");
}

void print_moves(Moves *moves)
{
    for (int i = 0; i < moves->number_of_moves; i++)
    {
        printf("%d.", i);
        print_move(moves->moves[i]);
    }
}


void pawn_moves(Chessboard *board, Moves *moves)
{
    bitboard bb;
    int piece, source_square, target_square;
    if (get_side(board) == white)
    {
        piece = P;
        bb = get_piece_bb(board, piece);
        for (source_square = LSB(bb); bb; pop_bit(bb, source_square), source_square = LSB(bb))
        {
            // QUIET MOVES
            target_square = source_square + 8;
            // target square is on board and there isn't another piece on the target square
            if (target_square <= a8 && !get_bit(get_occupancy_bb(board, white_and_black), target_square))
            {
                // check for pawn promotion
                if (source_square <= a7 && source_square >= h7)
                {
                    add_move(moves, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                }
                else
                {
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                }
                // double push
                if (((source_square <= a2) && (source_square >= h2)) && (!get_bit(get_occupancy_bb(board, white_and_black), target_square + 8)))
                {
                    add_move(moves, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0, 0));
                }
            }
            // CAPTURE MOVES
            bitboard white_pawn_attacks = get_pawn_attack(white, source_square) & get_occupancy_bb(board, black);
            target_square = LSB(white_pawn_attacks);
            for (; white_pawn_attacks; pop_bit(white_pawn_attacks, target_square), target_square = LSB(white_pawn_attacks))
            {
                // promotion check
                if (source_square <= a7 && source_square >= h7)
                {
                    add_move(moves, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                }
                else
                {
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                }
            }
            // ENPASSANT
            if (get_enpassant(board) != no_square)
            {
                // pawn attack bitboard & with en_passant bitboard to see if pawn does enpassant
                bitboard enpassant_attacks = get_pawn_attack(white, source_square) & (1ULL << get_enpassant(board));
                // make sure enpassant capture available
                if (enpassant_attacks)
                {
                    int target_enpassant = LSB(enpassant_attacks);
                    add_move(moves, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                }
            }
        }
    } 
    else // black side
    {
        piece = p;
        bb = get_piece_bb(board, piece);
        for (source_square = LSB(bb); bb; pop_bit(bb, source_square), source_square = LSB(bb))
        {
            // QUIET MOVES
            target_square = source_square - 8;
            // target square is on board and there isn't another piece on the target square
            if (target_square >= h1 && !get_bit(get_occupancy_bb(board, white_and_black), target_square))
            {
                // check for pawn promotion
                if (source_square <= a2 && source_square >= h2)
                {
                    add_move(moves, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));
                }
                else
                {
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                }
                // double push check
                if ((source_square <= a7 && source_square >= h7) && (!get_bit(get_occupancy_bb(board, white_and_black), target_square - 8)))
                {
                    add_move(moves, encode_move(source_square, target_square - 8, piece, 0, 0, 1, 0, 0));
                }
            }

            // CAPTURE MOVES
            bitboard black_pawn_attacks = pawn_attack_table[black][source_square] & get_occupancy_bb(board, white);
            target_square = LSB(black_pawn_attacks);
            for (; black_pawn_attacks; pop_bit(black_pawn_attacks, target_square), target_square = LSB(black_pawn_attacks))
            {
                // promotion check
                if (source_square <= a2 && source_square >= h2)
                {
                    add_move(moves, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                    add_move(moves, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                }
                else
                {
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                }
            }
            // ENPASSANT
            if (get_enpassant(board) != no_square)
            {
                // pawn attack bitboard & with en_passant bitboard to see if pawn does enpassant
                bitboard enpassant_attacks = pawn_attack_table[black][source_square] & (1ULL << get_enpassant(board));
                // make sure enpassant capture available
                if (enpassant_attacks)
                {
                    int target_enpassant = LSB(enpassant_attacks);
                    add_move(moves, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                }
            }
        }
    }
}


void castle_moves(Chessboard *board, Moves *moves)
{

    if (get_side(board) == white)
    {
        // king side castling is available
        if (get_castling_rights(board) & wk)
        {
            // make sure square between king and king's rook are empty
            if (!get_bit(get_occupancy_bb(board, white_and_black), f1) && !get_bit(get_occupancy_bb(board, white_and_black), g1))
            {
                // make sure king and the f1 squares are not under attacks
                if (!is_square_attacked(board, e1, black) && !is_square_attacked(board, f1, black))
                    add_move(moves, encode_move(e1, g1, K, 0, 0, 0, 0, 1));
            }
        }
        // queen side castling is available
        if (get_castling_rights(board) & wq)
        {
            // make sure square between king and queen's rook are empty
            if (!get_bit(get_occupancy_bb(board, white_and_black), d1) &&
                !get_bit(get_occupancy_bb(board, white_and_black), c1) && 
                !get_bit(get_occupancy_bb(board, white_and_black), b1))
            {
                // make sure king and the d1 squares are not under attacks
                if (!is_square_attacked(board, e1, black) && !is_square_attacked(board, d1, black))
                    add_move(moves, encode_move(e1, c1, K, 0, 0, 0, 0, 1));
            }
        }
        return;
    }
   
    // black side
    // king side castling is available
    if (get_castling_rights(board) & bk)
    {
        if (!get_bit(get_occupancy_bb(board, white_and_black), f8) && !get_bit(get_occupancy_bb(board, white_and_black), g8))
        {
            if (!is_square_attacked(board, e8, white) && !is_square_attacked(board, f8, white))
            {
                add_move(moves, encode_move(e8, g8, k, 0, 0, 0, 0, 1));
            }
        }
    }
    // queen side castling is available
    if (get_castling_rights(board) & bq)
    {
        if (!get_bit(get_occupancy_bb(board, white_and_black), d8) &&
            !get_bit(get_occupancy_bb(board, white_and_black), c8) &&
            !get_bit(get_occupancy_bb(board, white_and_black), b8))
        {
            if (!is_square_attacked(board, e8, white) && !is_square_attacked(board, d8, white))
            {
                add_move(moves, encode_move(e8, c8, k, 0, 0, 0, 0, 1));
            }
        }
    }
}


void knight_moves(Chessboard* board, Moves* moves)
{    
    int piece = (get_side(board) == white) ? N:n;
    bitboard bb = get_piece_bb(board, piece);
    // for evevry knight 
    for (int source_square = LSB(bb);  bb;  pop_bit(bb, source_square), source_square = LSB(bb))
    {
        bitboard knight_moves = get_knight_attack(source_square) & (~get_occupancy_bb(board, get_side(board)));
        // for every attack 
        for (int target_square = LSB(knight_moves);  knight_moves;  pop_bit(knight_moves, target_square), target_square = LSB(knight_moves))
        {   
            int move =  encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0);
            if (get_bit(get_occupancy_bb(board, !get_side(board)), target_square))    // check if capture
                add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            else
                add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));    
        }
    }
}


void king_moves(Chessboard* board, Moves* moves)
{    
    int piece = (get_side(board) == white) ? K:k;
    bitboard bb =  get_piece_bb(board, piece);
    int source_square = LSB(bb);
    bitboard king_moves = get_king_attack(source_square) & (~get_occupancy_bb(board, get_side(board)));
    // for every attack 
    for (int target_square = LSB(king_moves);  king_moves;  pop_bit(king_moves, target_square), target_square = LSB(king_moves))
    {   
        if (get_bit(get_occupancy_bb(board, !get_side(board)), target_square))    // check if capture
            add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
        else
            add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
    }
}


void bishop_moves(Chessboard* board, Moves* moves)
{
    int piece = (get_side(board) == white) ? B:b;
    bitboard bb =  get_piece_bb(board, piece);
    // for evevry bishop 
    for (int source_square = LSB(bb);  bb;  pop_bit(bb, source_square), source_square = LSB(bb))
    {
        bitboard bishop_moves = get_bishop_attack(source_square, get_occupancy_bb(board, white_and_black)) & (~get_occupancy_bb(board, get_side(board)));
        // for every attack 
        for (int target_square = LSB(bishop_moves);  bishop_moves;  pop_bit(bishop_moves, target_square), target_square = LSB(bishop_moves))
        {   
            if (get_bit(get_occupancy_bb(board, !get_side(board)), target_square))    // check if capture
                add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            else
                add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
        }
    }
}


void rook_moves(Chessboard* board, Moves* moves)
{
    int piece = (get_side(board) == white) ? R:r;
    bitboard bb =  get_piece_bb(board, piece);
    // for evevry bishop 
    for (int source_square = LSB(bb);  bb;  pop_bit(bb, source_square), source_square = LSB(bb))
    {
        bitboard rook_moves = get_rook_attack(source_square, get_occupancy_bb(board, white_and_black)) & (~get_occupancy_bb(board, get_side(board)));
        // for every attack 
        for (int target_square = LSB(rook_moves);  rook_moves;  pop_bit(rook_moves, target_square), target_square = LSB(rook_moves))
        {   
            if (get_bit(get_occupancy_bb(board, !get_side(board)), target_square))    // check if capture
                add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            else
                add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
        }
    }
}


void queen_moves(Chessboard* board, Moves* moves)
{
    int piece = (get_side(board) == white ? Q:q);
    bitboard bb =  get_piece_bb(board, piece);
    // for evevry bishop 
    for (int source_square = LSB(bb);  bb;  pop_bit(bb, source_square), source_square = LSB(bb))
    {
        bitboard queen_moves = get_queen_attack(source_square, get_occupancy_bb(board, white_and_black)) & (~ get_occupancy_bb(board, get_side(board)));
        // for every attack 
        for (int target_square = LSB(queen_moves);  queen_moves;  pop_bit(queen_moves, target_square), target_square = LSB(queen_moves))
        {   
            if (get_bit(get_occupancy_bb(board, !get_side(board)), target_square))    // check if capture
                add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            else
                add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
        }
    }
}


int generate_moves(Chessboard* board, Moves* moves)
{
    moves->number_of_moves = 0;
    pawn_moves(board, moves);
    knight_moves(board, moves);
    bishop_moves(board, moves);
    rook_moves(board, moves);
    queen_moves(board, moves);
    king_moves(board, moves);
    castle_moves(board, moves);
    return moves->number_of_moves;
}






