#include <stdio.h>
#include <stdlib.h>

#include "../board/board.h"

int main(void) {
    board_t *board = create_board();

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    print_board(board);

    do_move(board, 1L << 44, BLACK);
    print_board(board);

    do_move(board, 1L << 45, WHITE);
    print_board(board);

    free(board);

    return 0;
}
