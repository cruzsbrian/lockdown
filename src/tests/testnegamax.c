#include <stdio.h>

#include "../board/board.h"
#include "../search/negamax.h"

int main(void) {
    move_score_t next_move;

    board_t *board = create_board();
    board->b = 0L;
    board->w = 0L;

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    print_board(board);

    next_move = negamax(board, BLACK, 10);
    printf("%d\n", next_move.pos);

    do_move(board, 1L << next_move.pos, BLACK);
    print_board(board);

    next_move = negamax(board, WHITE, 10);
    printf("%d\n", next_move.pos);

    do_move(board, 1L << next_move.pos, WHITE);
    print_board(board);

    return 0;
}
