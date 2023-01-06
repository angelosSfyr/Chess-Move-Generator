#include "chessboard.h"
#include "bitboard.h"
#include "moves.h"

// Chess board stuff
char* uni_pieces[12] = 
{"♟︎", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"}; 

const char* square_to_coordinates[64] = {
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",
    "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",
    "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",
    "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",
    "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",
    "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",
    "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"
};

void reset_board(Chessboard* board)
{
    memset(board->bitboard, 0ULL, sizeof(board->bitboard));
    memset(board->occupancy, 0ULL, sizeof(board->occupancy));
    board->side = 0;
    board->enpassant = no_square;
    board->castling_rights = 0;    
}

void reset_occupancies(Chessboard* board)
{
    memset(board->occupancy, 0ULL, 24);
    for (int piece = P; piece <= K; piece++)
    {
        board->occupancy[black] |= board->bitboard[piece+6];
        board->occupancy[white] |= board->bitboard[piece];
    }     
    board->occupancy[white_and_black] |= board->occupancy[white];
    board->occupancy[white_and_black] |= board->occupancy[black];
}


void parse_fen(Chessboard* board, char const *fen)
{
    reset_board(board);
    for (int square = 63; *fen != ' '; fen++)
    {
        int piece = -1;
        switch (*fen) 
        {
            case 'p': piece = p; break;
            case 'r': piece = r; break;
            case 'n': piece = n; break;
            case 'b': piece = b; break;
            case 'k': piece = k; break;
            case 'q': piece = q; break;
            case 'P': piece = P; break;
            case 'R': piece = R; break;
            case 'N': piece = N; break;
            case 'B': piece = B; break;
            case 'K': piece = K; break;
            case 'Q': piece = Q;
        }
        if (((*fen) >= 'A' && (*fen) <= 'Z') || (((*fen) >= 'a' && (*fen) <= 'z')))
        {
            set_bit(board->bitboard[piece], square);
            square--;
        }
        if (((*fen) >= '1') && ((*fen) <= '8'))
        {
            square = square - ((*fen) - '0');
        }
    }
    // side to move
    (*(++fen) == 'w') ? (board->side = white) : (board->side = black);
    // castling rights
    fen += 2;
    while (*fen != ' ')
    {
        switch (*fen)
        {
            case 'K':
                board->castling_rights |= wk;
                break;
            case 'Q':
                board->castling_rights |= wq;
                break;
            case 'k':
                board->castling_rights |= bk;
                break;
            case 'q':
                board->castling_rights |= bq;
                break;
            case '-':
                break;
        }
        fen++;
    }
    // enpassant
    fen++;
    if (*fen == '-')
    {
        board->enpassant = no_square;
    }
    else
    {   
        int file = fen[0] - 'a';
        int rank = fen[1] - '0';
        board->enpassant = rank*8-file-1;
    }
    reset_occupancies(board);
}


void print_uni_board(Chessboard* board)
{
    printf("\n");
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            //printf("SQUARE: %d", 63-square);
            if (!file)  printf("  %d ", 8-rank);
            int piece = -1;
            // loop over all piece bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                // if there is a piece on current square
                if (get_bit(board->bitboard[bb_piece], 63-square))
                    // get piece code
                    piece = bb_piece;
            }
            printf("[%s ]", ((piece == -1) ? " " : uni_pieces[piece]));
        }
        printf("\n");
    }
    printf("\n     a  b  c  d  e  f  g  h\n\n");
    printf("     Side:     %s\n", (!board->side ? "white" : "black"));
    printf("     Enpassant:     %s\n", ((board->enpassant != no_square) ? square_to_coordinates[board->enpassant] : "no"));
    printf("     %c\n", ((board->castling_rights & wk) ? 'K' : '-'));
    printf("     %c\n", ((board->castling_rights & wq) ? 'Q' : '-'));
    printf("     %c\n", ((board->castling_rights & bq) ? 'q' : '-'));
    printf("     %c\n", ((board->castling_rights & bk) ? 'k' : '-'));
}

/*
    returns 1 if the sqaure given is attacked by the given side
    1. the square is attacked by a pawn if 
*/
int is_square_attacked(Chessboard* board, int square, int side)
{   
    if (pawn_attack_table[side^1][square] & board->bitboard[(side == white) ? P : p])
        return 1;

    if (knight_attack_table[square] & board->bitboard[((side == white) ? N : n)])
        return 1;

    if (King_attack_table[square] & board->bitboard[((side == white) ? K : k)])
        return 1;

    if (get_bishop_attack(square, board->occupancy[white_and_black]) & board->bitboard[((side == white) ? B : b)])
        return 1; 
    
    if (get_rook_attack(square, board->occupancy[white_and_black]) & board->bitboard[((side == white) ? R : r)])
        return 1;

    if (get_queen_attack(square, board->occupancy[white_and_black]) & board->bitboard[((side == white) ? Q : q)])
        return 1;

    return 0;
}

void copy_chessboard(Chessboard* to, Chessboard* from)
{
    memcpy(to, from, sizeof(*from));
}

int get_side(Chessboard* board)
{
    return board->side;
}

int get_enpassant(Chessboard* board)
{
    return board->enpassant;
}

int get_castling_rights(Chessboard* board)
{
    return board->castling_rights;
}

bitboard get_piece_bb(Chessboard* board, int piece)
{
    return board->bitboard[piece];
}

bitboard get_occupancy_bb(Chessboard* board, int side)
{
    return board->occupancy[side];
}

//castle right update array
// is right for square = 63-bitsquare
// because of the square mapping

// castling rights update constants
const int castl_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};



int make_move(Chessboard* board, int move)
{
    Chessboard cp;
    copy_chessboard(&cp,  board);   // NEED TO FIX THIS, ENGINE ISNT SUPPOSED TO CHECK THAT HERE
    
    // parse move
    int source_square = get_move_source(move);
    int target_square = get_move_target(move);
    int piece = get_move_piece(move);
    int promoted_piece = get_move_promoted(move);
    int capture = get_move_capture(move);
    int double_push = get_move_double(move);
    int enpass = get_move_enpassant(move);
    int castling = get_move_castling(move);
    // move piece
    pop_bit(board->bitboard[piece], source_square);
    set_bit(board->bitboard[piece], target_square);
    // check other flags
    if (capture)    // find captured piece and pop its bitboards's square
    {
        int start_piece = (board->side == white)?p:P;
        int end_piece = start_piece + 5;
        for (int i = start_piece; i <= end_piece; i++)
        {
            // if there's a piece on the target square
            if (get_bit(board->bitboard[i], target_square))
            {
                pop_bit(board->bitboard[i], target_square);
                break;
            }
        }
        //printf("%s, %s")
    }   
    if (promoted_piece)
    {
        pop_bit(board->bitboard[piece], target_square);
        set_bit(board->bitboard[promoted_piece], target_square);
    }
    if (enpass)
    {
        ((board->side)== white) ? pop_bit(board->bitboard[p], target_square - 8) :
                                   pop_bit(board->bitboard[P], target_square + 8);
    } 
    board->enpassant = no_square; // reset
    if (double_push)
    {
        board->enpassant = target_square + (board->side == white ? -8:+8);
    } 
    if (castling)
    {
        // switch target square
        switch (target_square)
        {
            // white castles king side
            case (g1):
                // move H rook
                pop_bit(board->bitboard[R], h1);
                set_bit(board->bitboard[R], f1);
                break;
            
            // white castles queen side
            case (c1):
                // move A rook
                pop_bit(board->bitboard[R], a1);
                set_bit(board->bitboard[R], d1);
                break;
            
            // black castles king side
            case (g8):
                // move H rook
                pop_bit(board->bitboard[r], h8);
                set_bit(board->bitboard[r], f8);
                break;
            
            // black castles queen side
            case (c8):
                // move A rook
                pop_bit(board->bitboard[r], a8);
                set_bit(board->bitboard[r], d8);
                break;
        }
    }
    // update castling rights
    board->castling_rights &= castl_rights[63-source_square];
    board->castling_rights &= castl_rights[63-target_square];
    reset_occupancies(board);
    board->side ^= 1;
    // if king is in check restore board
    int king_to_be_attacked = (board->side == white)?k:K;
    
    if (is_square_attacked(board, LSB(board->bitboard[king_to_be_attacked]), board->side))
    {
        copy_chessboard(board, &cp);
        return 0;   // return illegal move
    }
    else
    {
        return 1;
    }
}
