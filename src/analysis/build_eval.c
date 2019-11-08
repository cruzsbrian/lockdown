#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../board/board.h"
#include "../search/search.h"
#include "../search/endgame.h"


#define PROGRESS_STEPS 20


void init_board(board_t *board);
void random_play(board_t *board, int c, int total_moves);

void record_game(board_t *board, int c, int16_t score, long n_nodes);

int sign(int16_t n);


/** 
 * Run many high speed games and searches to get training data for the eval
 * function. Args specify number of trials, search depth (ie depth the training
 * will be based on).
 */
int main(int argc, char *argv[]) {
    int n_trials, depth;

    board_t *board;
    int ii, color;
    move_score_t endgame_result;
    long n_nodes;

    int progress = 0;

    if (argc != 3) {
        fprintf(stderr, "Usage: build_eval [n_trials] [search_depth]\n");
        return 1;
    }

    n_trials = atoi(argv[1]);
    depth = atoi(argv[2]);

    srand(time(NULL));

    board = create_board();
    color = depth % 2;

    for (ii = 0; ii < n_trials; ++ii) {
        init_board(board);
        random_play(board, BLACK, 60 - depth);

        n_nodes = 0L;
        endgame_result = ab_ff(board, color, -1, 1, 0, 60, NULL, &n_nodes);

        record_game(board, color, endgame_result.score, n_nodes);

        if (ii * PROGRESS_STEPS > progress * n_trials) {
            progress++;
            fprintf(stderr, ".");
        }
    }

    fprintf(stderr, "\n");

    return 0;
}


void init_board(board_t *board) {
    board->b = 0x0000000810000000;
    board->w = 0x0000001008000000;
}

void random_play(board_t *board, int c, int total_moves) {
    int move_num = 0;

    while (move_num < total_moves) {
        uint64_t move_mask = get_moves(board, c);
        int moves[60];
        int n_moves = 0;

        while (move_mask) {
            int m = __builtin_ctzll(move_mask);
            move_mask &= move_mask - 1;

            moves[n_moves] = m;
            n_moves++;
        }

        if (n_moves) {
            do_move(board, moves[rand() % n_moves], c);
        }
        c = !c;
        move_num++;
    }
}


void record_game(board_t *board, int c, int16_t score, long n_nodes) {
    int move_num = popcount_board(board) - 4;
    printf("%016lx %016lx %d %d %d %ld\n",
            board->b,
            board->w,
            move_num,
            c,
            score,
            n_nodes);
}


int sign(int16_t n) {
    if (n > 0) return 1;
    if (n < 0) return -1;
    return 0;
}
