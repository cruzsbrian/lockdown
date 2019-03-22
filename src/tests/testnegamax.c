#include <stdio.h>

#include "../board/board.h"
#include "../search/negamax.h"

int main(void) {
    int next_move;

    board_t *board = create_board();
    board->b = 0L;
    board->w = 0L;

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    print_board(board);

    while (1) {
        next_move = negamax(board, BLACK, 8).pos;
        printf("%d\n", next_move);

        do_move(board, 1L << next_move, BLACK);
        print_board(board);

        printf("White move: ");
        if (!scanf("%d", &next_move)) {
            break;
        }

        do_move(board, 1L << next_move, WHITE);
        print_board(board);
    }

    return 0;
}
