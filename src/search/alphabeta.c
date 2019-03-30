#include "alphabeta.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"




move_score_t ab_search(board_t *board, int c, int depth, long *n) {
    move_score_t *moves;
    board_t old;
    size_t n_moves;
    float score;
    move_score_t best_move, move, result;
    int ii;

    get_scored_moves(&moves, &n_moves, board, c);

    best_move.pos = -1;
    best_move.score = -FLT_MAX;
    best_move.end = 0;

    if (n_moves == 0) {
        result = alphabeta(board, !c, -FLT_MAX, FLT_MAX, depth, n);
        best_move.score = -result.score;
        best_move.end = result.end;
        return best_move;
    }

    for (ii = 0; ii < n_moves; ++ii) {
        move = moves[ii];

        printf("Scanning move %2d ... ", move.pos);

        old = *board;
        do_move(board, move.pos, c);
        result = alphabeta(board, !c, -FLT_MAX, -best_move.score, depth, n);
        *board = old;

        score = -result.score;

        /* If move leads to guaranteed win, return it. */
        if (result.end && score > 0) {
            printf("score %.2f (guaranteed win)\n", score);

            best_move.pos = move.pos;
            best_move.score = score;
            best_move.end = 1;
            return best_move;
        }

        if (score > best_move.score) {
            printf("score %.2f\n", score);

            best_move.pos = move.pos;
            best_move.score = score;
            best_move.end = result.end;
        } else {
            printf("\n");
        }
    }

    return best_move;
}


move_score_t alphabeta(board_t *board, int c, float alpha, float beta, int depth, long *n) {
    uint64_t moves, old_b, old_w;
    int move;
    float score;
    move_score_t best_move, result;

    (*n)++;

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
        old_b = board->b;
        old_w = board->w;

        /* Make move and get its score. */
        do_move(board, move, c);
        result = alphabeta(board, !c, -beta, -alpha, depth - 1, n);
        score = -result.score;

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
            best_move.end = result.end;
            alpha = score;
        }
    }

    best_move.score = alpha;
    return best_move;
}




/**
 * Gets all the moves available to color c for board b, puts them in an array,
 * and sorts them. The resulting array is at *move_arr. The number of moves is
 * put into *n. *move_arr must be freed later.
 */
void get_scored_moves(move_score_t **move_arr, size_t *n, board_t *board, int c) {
    int move;
    uint64_t old_b, old_w;
    uint64_t moves_mask = get_moves(board, c);

    *move_arr = malloc(24 * sizeof(move_score_t));
    if (*move_arr == NULL) {
        fprintf(stderr, "Error: unable to allocate memory for move array.");
        exit(1);
    }

    *n = 0;
    while (moves_mask) {
        move = __builtin_ctzll(moves_mask);
        moves_mask &= moves_mask - 1;

        (*move_arr)[*n].pos = move;

        old_b = board->b;
        old_w = board->w;

        do_move(board, move, c);
        (*move_arr)[*n].score = table_eval(board, c);

        board->b = old_b;
        board->w = old_w;

        (*n)++;
    }

    qsort(*move_arr, *n, sizeof(move_score_t), compare_moves);
}


int compare_moves(const void *m1, const void *m2) {
    float s1 = ((move_score_t *)m1)->score;
    float s2 = ((move_score_t *)m2)->score;
    return (int)(s2 - s1);
}
