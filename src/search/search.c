#include "search.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"
#include "alphabeta.h"


const int DEPTH = 9;
const int ENDGAME_MOVES = 20;


int ab_search(board_t *board, int c, int depth, long *n);
int endgame_search(board_t *board, int c, long *n);


int search(board_t *board, int c, int move_num) {
    int result;
    long n_nodes = 0L;
    clock_t start, end;
    float seconds, nps;

    start = clock();

    if (move_num >= 60 - ENDGAME_MOVES) {
        fprintf(stderr, "Running end-game solver.\n");
        result = endgame_search(board, c, &n_nodes);
    } else {
        fprintf(stderr, "Running alphabeta search.\n");
        result = ab_search(board, c, DEPTH, &n_nodes);
    }

    end = clock();

    /* Convert search time to seconds, find nodes per second, print. */
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    nps = (float)n_nodes / seconds;
    fprintf(stderr, "%ld nodes in %.2fs @ %.0f node/s\n",
            n_nodes, seconds, nps);

    return result;
}


int ab_search(board_t *board, int c, int depth, long *n) {
    board_t old;
    move_score_t *moves;
    size_t n_moves;

    float best_score, score;
    int best_move, move;
    move_score_t result;

    int ii;

    /* Get available moves, sorted by eval score. */
    get_scored_moves(&moves, &n_moves, board, c);

    /* Start with minimum score */
    best_score = -FLT_MAX;
    best_move = -1;

    /* If there are no available moves, return a pass. */
    if (n_moves == 0) {
        return -1;
    }

    /* Search each move. */
    for (ii = 0; ii < n_moves; ++ii) {
        move = moves[ii].pos;

        fprintf(stderr, "Scanning move %2d ... ", move);

        /*
         * Store old board, apply the move, evaluate with alphabeta, then revert
         * to the stored board.
         */
        old = *board;
        do_move(board, move, c);
        result = alphabeta(board, !c, -FLT_MAX, -best_score, depth, n);
        *board = old;

        /* Score from alphabeta will be for the opponent. */
        score = -result.score;

        /* If move leads to guaranteed win, return it. */
        if (result.end && score > 0) {
            fprintf(stderr, "score %.2f (guaranteed win)\n", score);
            return move;
        }

        /*
         * If this move is the best one found yet, print score and update
         * best_move. Otherwise just print newline and go to next.
         */
        if (score > best_score) {
            fprintf(stderr, "score %.2f\n", score);

            best_move = move;
            best_score = score;
        } else {
            fprintf(stderr, "\n");
        }
    }

    /* Free the array of moves allocated by get_scored_moves(). */
    free(moves);

    return best_move;
}


int endgame_search(board_t *board, int c, long *n) {
    board_t old;
    move_score_t *moves;
    size_t n_moves;
    float best_score, score;
    int move;
    move_score_t result;
    int ii;

    /* Get available moves, sorted by eval score. */
    get_scored_moves(&moves, &n_moves, board, c);

    best_score = -1;

    /* If there are no available moves, pass. */
    if (n_moves == 0) {
        return -1;
    }

    for (ii = 0; ii < n_moves; ++ii) {
        move = moves[ii].pos;

        fprintf(stderr, "Scanning move %2d ... ", move);

        /*
         * Store old board, apply the move, evaluate with alphabeta, then revert
         * to the stored board.
         */
        old = *board;
        do_move(board, move, c);
        result = alphabeta(board, !c, -1, -best_score, 60, n);
        *board = old;

        score = -result.score;

        /* Print for win/draw/loss. If a win, immediately return. */
        if (score > 0) {
            fprintf(stderr, "win\n");
            return move;
        } else if (score == 0) {
            fprintf(stderr, "draw\n");
            best_score = score;
        } else {
            fprintf(stderr, "loss\n");
        }
    }

    return move;
}
