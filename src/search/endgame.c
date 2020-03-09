#include "endgame.h"

#include <stdio.h>
#include <stdlib.h>
#include "../eval/table_eval.h"
#include "alphabeta.h"


#define SORT_CUTOFF 5
#define MAX_MOVES 32


move_score_t ab_ff(board_t *board, int c, int16_t alpha, int16_t beta,
                    int depth, int max_depth, int move_num, node_t *tt, long *n) {
    uint64_t move_mask;
    board_t new_boards[MAX_MOVES];
    int16_t mobilities[MAX_MOVES];
    int8_t moves[MAX_MOVES];
    int16_t score;
    move_score_t best_move;
    int8_t move;
    int ii, n_moves;

    (*n)++;

    if (max_depth - depth < SORT_CUTOFF) {
        return ab(board, c, alpha, beta, depth, max_depth, move_num, tt, n, 0);
    }

    if (depth == max_depth) {
        best_move.score = table_eval(board, c, move_num);
        return best_move;
    }

    move_mask = get_moves(board, c);
    if (move_mask == 0L) {
        /* If game is over (no player can move). */
        if (get_moves(board, !c) == 0L) {
            /* print_board(board); */
            best_move.score = endgame_eval(board, c);
            /* fprintf(stderr, "score %d\n\n", endgame_eval(board, WHITE)); */
            best_move.end = 1;
        } else { /* Otherwise we just pass. */
            best_move.score = -ab_ff(board, !c, -beta, -alpha, depth, max_depth, move_num, tt, n).score;
        }

        best_move.pos = -1;

        return best_move;
    }

    /* Go through moves and find resulting boards and opponent mobillities. */
    n_moves = 0;
    while (move_mask != 0ULL) {
        board_t old;

        move = __builtin_ctzll(move_mask);
        move_mask &= move_mask - 1;

        old = *board;
        do_move(board, move, c);

        new_boards[n_moves] = *board;
        mobilities[n_moves] = popcount(get_moves(board, !c));
        moves[n_moves] = move;

        *board = old;

        n_moves++;
    }

    best_move.score = -INT16_MAX;
    best_move.pos = -1;

    /* Loop through all moves. */
    for (ii = 0; ii < n_moves; ++ii) {
        int16_t min_mobility = INT16_MAX;
        int best_index, jj;

        /* Find fastest move to search. */
        for (jj = 0; jj < n_moves; ++jj) {
            int16_t mob = mobilities[jj];
            if (mob < min_mobility) {
                min_mobility = mob;
                best_index = jj;
            }
        }

        /* This will prevent searching this move next time. */
        mobilities[best_index] = INT16_MAX;

        score = -ab_ff(new_boards + best_index, !c, -beta, -alpha, depth + 1, max_depth, move_num + 1, tt, n).score;

        if (score >= beta) {
            best_move.score = score;
            best_move.pos = moves[best_index];
            return best_move;
        }

        if (score > best_move.score) {
            best_move.score = score;
            best_move.pos = moves[best_index];

            if (score > alpha) {
                alpha = score;
            }
        }
    }

    return best_move;
}
