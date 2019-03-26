#include "alphabeta.h"

#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"


move_score_t alphabeta(board_t *board, int c, float alpha, float beta, int depth) {

    if (depth == 0) {
        /* TODO: add quiescence */
        result.pos = -1;
        result.score = table_eval(board, c);
        return result;
    }

    uint64_t moves, old_b, old_w;
    int move, score;
    move_score_t best_move;

    moves = get_moves(board, c);

    if (moves == 0L && get_moves(board, !c) == 0L) {
        result.pos = -1;
        result.score = table_eval(board, c);
        return result;
    }

    best_move.pos = -1;
    best_move.score = -FLT_MAX;

    if (moves == 0L) {
        max_score = -negamax(board, !c, -beta, -alpha, depth - 1);
    }

    while (moves) {

        /* Get index of least significant bit. */
        move = __builtin_ctzll(moves);

        /* Zero out least significant bit. */
        moves &= moves - 1;

        /* Store old board for undoing move. */
        old_b = board->b;
        old_w = board->w;

        /* Make move and get its score. */
        do_move(board, move, c);
        score = -alphabeta(board, !c, -beta, -alpha, depth - 1).score;

        /* Undo move. */
        board->b = old_b;
        board->w = old_w;

        if (score > best_move.score) {
            best_move.score = score;
            best_move.move = move;
        }

        if (best_move.score > alpha) {
            alpha = best_move.score;
        }

        if (best_move.score >= beta) {
            break;
        }
    }

    return best_move;

}
