#include "alphabeta.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"


/**
 * Alphabeta:
 * Run the alphabeta minimax algorithm to recursively evaluate a board to a
 * specified depth. Returns a move_score_t containing the best move, its score,
 * and whether the game finished. Parameter n is used to track total number of
 * nodes visited.
 */
move_score_t alphabeta(board_t *board, int c, float alpha, float beta, int depth, long *n) {
    board_t old;
    uint64_t moves;
    int move;
    float score;
    move_score_t best_move, result;

    /* Increment visited node counter. */
    (*n)++;

    /* Initialize best move to pass, did not reach end of game. */
    best_move.pos = -1;
    best_move.end = 0;

    /* If depth reached, evaluate and return. */
    if (depth == 0) {
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
        result = alphabeta(board, !c, -beta, -alpha, depth - 1, n);
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
        result = alphabeta(board, !c, -beta, -alpha, depth - 1, n);
        score = -result.score;

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
            best_move.end = result.end;
            alpha = score;
        }
    }

    best_move.score = alpha;
    return best_move;
}
