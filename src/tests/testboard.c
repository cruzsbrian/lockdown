#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <x86intrin.h>

#include "../board/board.h"
#include "../search/search.h"
#include "../search/alphabeta.h"

int main(void) {
    move_score_t *moves;
    size_t n_moves;
    int ii;

    board_t *board = create_board();
    board->b = 0x0000000808202400;
    board->w = 0x00000010161f1b3f;

    print_board(board);

    free(board);

    printf("%ld", sizeof(float));

    /*printf("%d\n", _pext_u32(0xffffffff, 0x00000005));*/

    return 0;
}
