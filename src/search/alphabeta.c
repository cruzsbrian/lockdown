#include "alphabeta.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "../board/board.h"
#include "../eval/table_eval.h"
#include "trans_table.h"


/**
 * Alphabeta:
 * Run the alphabeta minimax algorithm to recursively evaluate a board to a
 * specified depth. Returns a move_score_t containing the best move, its score,
 * and whether the game finished. Parameter n is used to track total number of
 * nodes visited.
 */
move_score_t alphabeta(board_t *board, int c, int16_t alpha, int16_t beta,
                       int depth, int max_depth, node_t *tt, long *n, int use_tt) {
    board_t old;
    uint64_t moves;
    int move;
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
        result = alphabeta(board, !c, -beta, -alpha, depth + 1, max_depth, tt, n, use_tt);
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

        /* Store old board for undo. */
        old = *board;

        /* Make move and get its score. */
        do_move(board, move, c);
        result = alphabeta(board, !c, -beta, -alpha, depth + 1, max_depth, tt, n, use_tt);
        score = -result.score;

        if (depth == 1 && use_tt) {
            set_score(tt, *board, !c, result.score, max_depth - depth, NODE_EXACT);
        }

        /* Undo move. */
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
