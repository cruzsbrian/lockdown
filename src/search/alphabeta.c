#include "alphabeta.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "../board/board.h"
#include "../eval/table_eval.h"
#include "trans_table.h"
#include "move_ordering.h"


#define SORT_CUTOFF 6
#define MAX_MOVES 32


/**
 * Alphabeta:
 * Run the alphabeta minimax algorithm to recursively evaluate a board to a
 * specified depth. Returns a move_score_t containing the best move, its score,
 * and whether the game finished. Parameter n is used to track total number of
 * nodes visited.
 */
move_score_t ab_ordered(board_t *board, int c, int16_t alpha, int16_t beta,
                    int depth, int max_depth, node_t *tt, long *n) {
    uint64_t move_mask;
    board_t new_boards[MAX_MOVES];
    int16_t scores[MAX_MOVES];
    int8_t moves[MAX_MOVES];
    int16_t score;
    move_score_t best_move;
    int8_t move;
    int ii, n_moves;

    (*n)++;

    if (max_depth - depth < SORT_CUTOFF) {
        return ab(board, c, alpha, beta, depth, max_depth, tt, n, 0);
    }

    if (depth == max_depth) {
        best_move.score = table_eval(board, c);
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
            best_move.score = -ab_ordered(board, !c, -beta, -alpha, depth, max_depth, tt, n).score;
        }

        best_move.pos = -1;

        return best_move;
    }

    /* Go through moves and find resulting boards and scores. */
    n_moves = 0;
    while (move_mask != 0ULL) {
        board_t old;

        move = __builtin_ctzll(move_mask);
        move_mask &= move_mask - 1;

        old = *board;
        do_move(board, move, c);

        new_boards[n_moves] = *board;
        scores[n_moves] = find_score(board, c, SORT_CUTOFF - depth, tt, n);
        moves[n_moves] = move;

        *board = old;

        n_moves++;
    }

    best_move.score = -INT16_MAX;
    best_move.pos = -1;

    /* Loop through all moves. */
    for (ii = 0; ii < n_moves; ++ii) {
        int16_t max_score = -INT16_MAX;
        int best_index, jj;

        /* Find fastest move to search. */
        for (jj = 0; jj < n_moves; ++jj) {
            int16_t s = scores[jj];
            if (s > max_score) {
                max_score = s;
                best_index = jj;
            }
        }

        /* This will prevent searching this move next time. */
        scores[best_index] = -INT16_MAX;

        score = -ab_ordered(new_boards + best_index, !c, -beta, -alpha, depth + 1, max_depth, tt, n).score;
        set_score(tt, *board, !c, -score, max_depth - depth, NODE_EXACT);

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


move_score_t ab(board_t *board, int c, int16_t alpha, int16_t beta,
                   int depth, int max_depth, node_t *tt, long *n, int use_tt) {
    board_t old;
    uint64_t moves;
    int8_t move;
    int16_t score;
    move_score_t best_move, result;

    /* Increment visited node counter. */
    (*n)++;

    /* Initialize best move to pass, did not reach end of game. */
    best_move.pos = -1;
    best_move.end = 0;

    /* If depth reached, evaluate and return. */
    if (depth == max_depth) {
        best_move.score = table_eval(board, c);
        return best_move;
    }

    moves = get_moves(board, c);

    /* If game over, evaluate and return. */
    if (moves == 0L && get_moves(board, !c) == 0L) {
        best_move.score = endgame_eval(board, c);
        best_move.end = 1;
        return best_move;
    }

    /* If no moves available, pass and continue down. */
    if (moves == 0L) {
        result = ab(board, !c, -beta, -alpha, depth + 1, max_depth, tt, n, use_tt);
        score = -result.score;
        if (score >= beta) {
            best_move.score = beta;
            return best_move;
        } else {
            best_move.score = score;
            best_move.end = result.end;
            return best_move;
        }
    }

    while (moves) {
        /* Get index of least significant bit and then zero it out. */
        move = __builtin_ctzll(moves);
        moves &= moves - 1;

        old = *board;
        do_move(board, move, c);
        result = ab(board, !c, -beta, -alpha, depth + 1, max_depth, tt, n, use_tt);
        score = -result.score;
        *board = old;

        /*
         * If score is above beta, opponent won't allow this board state to be
         * reached, so we can stop searching it.
         */
        if (score >= beta) {
            best_move.score = beta;
            return best_move;
        }

        /* If score is above alpha, record it as best and set alpha to score. */
        if (score > alpha) {
            best_move.pos = move;
            alpha = score;
        }
    }

    best_move.score = alpha;
    return best_move;
}
