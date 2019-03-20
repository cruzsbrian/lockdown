#include <stdio.h>

#include "../board/board.h"

int main(void) {
    board_t *board = create_board();

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    print_board(board);

    return 0;
}
