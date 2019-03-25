#include "alphabeta.h"

#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"


move_score_t alphabeta(board_t *board, int c, float alpha, float beta, int depth) {
    uint64_t moves, old_b, old_w;
    int move, score;
    move_score_t best_move;

    if (depth == 0) {
        result.pos = -1;
        result.score = table_eval(board, c);
        return result;
    }

    moves = get_moves(board, c);

    if (moves == 0L && get_moves(board, !c) == 0L) {
        result.pos = -1;
        result.score = table_eval(board, c);
        return result;
    }

    best_move.pos = -1;
    best_move.score = beta;

    if (moves == 0L) {
        max_score = -negamax(board, !c, -beta, -alpha, depth - 1);
    }

    while (moves) {
        move = __builtin_ctzll(moves);

        moves &= moves - 1;

        do_move(board, move, c);
        score = -alphabeta(board, !c, -beta, -alpha, depth - 1).score;

        if (score > alpha)
