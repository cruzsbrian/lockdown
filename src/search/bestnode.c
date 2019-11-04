#include "bestnode.h"

#include <stddef.h>
#include "../board/board.h"
#include "search.h"
#include "move_ordering.h"
#include "alphabeta.h"

#include <assert.h>

int16_t next_guess(int16_t alpha, int16_t beta, int subtrees);


int8_t bns(board_t *board, int c, int depth, node_t *tt, long *n) {
    board_t old;
    move_score_t *moves;
    size_t n_moves;
    int better_count;

    int16_t score, test_score, alpha, beta;
    int8_t move, best_move;

    int ii;

    get_scored_moves(&moves, &n_moves, board, c, 5, tt, n);
    better_count = (int) n_moves;
    best_move = moves[0].pos;

    test_score = -alphabeta(board, c, -INT16_MAX, INT16_MAX, 0, depth / 2, tt, n, 1).score;

    alpha = test_score - 100;
    beta = test_score + 100;

    while (beta - alpha >= 2 && better_count != 1) {
        better_count = 0;

        for (ii = 0; ii < n_moves; ++ii) {
            move = moves[ii].pos;

            old = *board;
            do_move(board, move, c);
            score = -alphabeta(board, !c, -beta, -alpha, 0, depth, tt, n, 1).score;
            *board = old;

            if (score > test_score) {
                better_count++;
                best_move = move;
            }
        }

       if (better_count) {
           alpha = test_score;
       } else {
           beta = test_score;
           better_count = n_moves;
       }

       test_score = next_guess(alpha, beta, better_count);
    }

    return best_move;
}


int16_t next_guess(int16_t alpha, int16_t beta, int subtrees) {
    return alpha + (beta - alpha) * (subtrees - 1) / subtrees;
}
