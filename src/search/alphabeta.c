#include "alphabeta.h"

#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"


move_score_t alphabeta(board_t *board, int c, float alpha, float beta, int depth) {
    uint64_t moves, old_b, old_w;
    int move;
    float score;
    move_score_t best_move;

    best_move.pos = -1;

    /* If depth reached, evaluate and return. */
    if (depth == 0) {
        best_move.score = table_eval(board, c);
        return best_move;
    }

    moves = get_moves(board, c);

    /* If game over, evaluate and return. */
    if (moves == 0L && get_moves(board, !c) == 0L) {
        best_move.score = table_eval(board, c);
        return best_move;
    }

    /* If no moves available, pass and continue down. */
    if (moves == 0L) {
        score = -alphabeta(board, !c, -beta, -alpha, depth - 1).score;
        if (score >= beta) {
            best_move.score = beta;
            return best_move;
        } else {
            best_move.score = score;
            return best_move;
        }
    }

    while (moves) {
        /* Get index of least significant bit and then zero it out. */
        move = __builtin_ctzll(moves);
        moves &= moves - 1;

        /* Store old board for undo. */
        old_b = board->b;
        old_w = board->w;

        /* Make move and get its score. */
        do_move(board, move, c);
        score = -alphabeta(board, !c, -beta, -alpha, depth - 1).score;
        
        /* Undo move. */
        board->b = old_b;
        board->w = old_w;

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
