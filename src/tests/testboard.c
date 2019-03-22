#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

#include "../board/board.h"
#include "../eval/simple_eval.h"
#include "../search/negamax.c"

int main(void) {
    board_t *board = create_board();
    board->b = 0x0000000808202400;
    board->w = 0x00000010161f1b3f;

    print_board(board);

    printf("%f\n", simple_eval(board, BLACK));

    print_bits(get_moves(board, BLACK));

    free(board);


    /*printf("%d\n", _pext_u32(0xffffffff, 0x00000005));*/

    return 0;
}
