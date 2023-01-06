# Chess-Move-Generator
A pseudo-legal chess move generator utilizing bitboards to represent the current state of the board is presented.

# Pseudo-legal generation
To generate a list of pseudo-legal moves, the program first needs to generate a list of all possible moves for each piece on the board.
The function make_move is provided, which attempts to execute a move on the chessboard and returns a boolean indicating the legality of the move.
If the move is illegal, make_move restores the board to its previous position and returns false. Otherwise, it returns true.

# ABout BitBoards
![Alt text](./Bitboard.gif)
A bitboard is a data structure used in computer chess programming to represent the state of the board.
It is a long string of bits (ones and zeros) that can be manipulated using bitwise operations to quickly and efficiently generate a list of legal moves.
Each square on the chessboard is represented by a single bit in the bitboard. If the bit is a 1, it means the square is occupied by a piece.
If the bit is a 0, it means the square is empty.
