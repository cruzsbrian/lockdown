#include <stdio.h>

#include <stdlib.h>
#include "../board/board.h"
#include "../search/negamax.h"
#include "../eval/table_eval.h"

void testPosition(void);

int main(void) {
    testPosition();
    return 0;
}

void testPosition(void) {
    move_score_t result;
    uint64_t moves;

    board_t *board = create_board();
    board->b = 0x00004a5020003320;
    board->w = 0xff7f352fdfff0c0f;

    print_board(board);
    moves = get_moves(board, BLACK);
    print_bits(moves);

    result = negamax(board, BLACK, 3);
    printf("Black move: %d\n", result.pos);
    printf("Score: %.2f\n", -result.score);

    do_move(board, result.pos, BLACK);
    print_board(board);

    free(board);
}
