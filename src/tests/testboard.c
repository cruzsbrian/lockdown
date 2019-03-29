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
    board->b = 0L;
    board->w = 0L;

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);
    add_piece(board, 26, WHITE);

    print_board(board);

    print_bits(get_flippable(board, BLACK));

    free(board);

    /*printf("%d\n", _pext_u32(0xffffffff, 0x00000005));*/

    return 0;
}
