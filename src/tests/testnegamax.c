#include <stdio.h>

#include "../board/board.h"
#include "../search/negamax.h"
#include "../eval/table_eval.h"

void testPosition(void);
void playGame(void);

int main(void) {
    playGame();
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
}

void playGame(void) {
    move_score_t result;
    int next_move;
    int moves_remaining = 60;

    board_t *board = create_board();
    board->b = 0L;
    board->w = 0L;

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    print_board(board);

    while (1) {
        if (moves_remaining <= 14) {
            result = negamax(board, BLACK, moves_remaining + 1);
        } else {
            result = negamax(board, BLACK, 7);
        }
        next_move = result.pos;

        if (next_move == -1) {
            printf("Black move: pass\n");
        } else {
            printf("Black move: %d\n", next_move);
            do_move(board, 1L << next_move, BLACK);
        }

        print_board(board);

        do {
            printf("White move: ");
            if (!scanf("%d", &next_move)) {
                next_move = 0L;
            }
        } while (!((1L << next_move) & get_moves(board, WHITE)));

        do_move(board, 1L << next_move, WHITE);
        print_board(board);

        moves_remaining -= 2;
    }
}
