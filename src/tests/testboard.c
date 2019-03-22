#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

#include "../board/board.h"
#include "../eval/simple_eval.h"

int main(void) {
    board_t *board = create_board();
    board->b = 0L;
    board->w = 0L;

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    print_board(board);

    printf("%f\n", simple_eval(board, BLACK));

    do_move(board, 1L << 44, BLACK);
    print_board(board);

    printf("%f\n", simple_eval(board, BLACK));

    do_move(board, 1L << 45, WHITE);
    print_board(board);

    printf("%f\n", simple_eval(board, BLACK));

    print_bits(moves(board, BLACK));

    free(board);


    printf("%d\n", _pext_u32(0xffffffff, 0x00000005));

    return 0;
}
