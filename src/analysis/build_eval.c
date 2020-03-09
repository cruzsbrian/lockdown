#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../board/board.h"
#include "../search/search.h"
#include "../search/endgame.h"
#include "../eval/table_eval.h"


#define PROGRESS_STEPS 20


void init_board(board_t *board);
void random_play(board_t *board, int c, int total_moves);

void record_game(board_t *board, int c, int16_t score);

int sign(int16_t n);


/** 
 * Run many high speed games and searches to get training data for the eval
 * function. Args specify number of trials, search depth (ie depth the training
 * will be based on).
 */
int main(int argc, char *argv[]) {
    int n_trials, start_depth, search_depth;

    board_t *board;
    int ii, color;
    move_score_t endgame_result;
    long n_nodes;

    int progress = 0;

    if (argc != 4) {
        fprintf(stderr, "Usage: build_eval [n_trials] [start_depth] [search_depth]\n");
        return 1;
    }

    n_trials = atoi(argv[1]);
    start_depth = atoi(argv[2]);
    search_depth = atoi(argv[3]);

    srand(time(NULL));

    board = create_board();
    color = start_depth % 2;

    for (ii = 0; ii < n_trials; ++ii) {
        init_board(board);
        random_play(board, BLACK, start_depth);

        n_nodes = 0L;
        endgame_result = ab_ff(board, color, -INT16_MAX, INT16_MAX, 0, search_depth, NULL, &n_nodes);

        record_game(board, color, endgame_result.score);

        if (ii * PROGRESS_STEPS > progress * n_trials) {
            progress++;
            fprintf(stderr, ".");
        }
    }

    /* fprintf(stderr, "\n"); */

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


void record_game(board_t *board, int c, int16_t score) {
    uint64_t own, opp, own_moves, opp_moves, own_flip, opp_flip;

    if (c == BLACK) {
        own = board->b;
        opp = board->w;
    } else {
        own = board->w;
        opp = board->b;
    }

    get_moves_flips(&own_moves, &own_flip, board, c);
    get_moves_flips(&opp_moves, &opp_flip, board, !c);

    int move_num = popcount_board(board) - 4;

    printf("%016lx %016lx %d %d %d\n",
            board->b,
            board->w,
            move_num,
            c,
            score);

    printf("%d %d %d %d %d %d %d %d\n",
            n_corners(own) - n_corners(opp),
            n_edges(own) - n_edges(opp),
            popcount(own_moves) - popcount(opp_moves),
            popcount(own_flip) - popcount(opp_flip),
            get_frontier(board, c) - get_frontier(board, !c),
            x_square(own) - x_square(opp),
            c_square(own) - c_square(opp),
            get_stable(board, c) - get_stable(board, !c));
}


int sign(int16_t n) {
    if (n > 0) return 1;
    if (n < 0) return -1;
    return 0;
}
