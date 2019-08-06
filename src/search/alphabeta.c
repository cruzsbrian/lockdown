#include "alphabeta.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"




/**
 * Alphabeta-search:
 * Top-level function to find moves with a given search depth. Sorts moves using
 * the eval function, then calls alphabeta() on each one to recursively search
 * it. Parameter n is used for tracking the number of nodes visited.
 */
move_score_t ab_search(board_t *board, int c, int depth, long *n) {
    move_score_t *moves;
    board_t old;
    size_t n_moves;
    float score;
    move_score_t best_move, move, result;
    int ii;

    /* Get available moves, sorted by eval score. */
    get_scored_moves(&moves, &n_moves, board, c);

    /*
     * Initial best_move before search: pass, minimum score, did not reach
     * end-game.
     */
    best_move.pos = -1;
    best_move.score = -FLT_MAX;
    best_move.end = 0;

    /*
     * If there are no available moves, return an accurate score and endgame
     * flag with a pass.
     */
    if (n_moves == 0) {
        result = alphabeta(board, !c, -FLT_MAX, FLT_MAX, depth, n);
        best_move.score = -result.score;
        best_move.end = result.end;
        return best_move;
    }

    /* Search each move. */
    for (ii = 0; ii < n_moves; ++ii) {
        move = moves[ii];

        fprintf(stderr, "Scanning move %2d ... ", move.pos);

        /*
         * Store old board, apply the move, evaluate with alphabeta, then revert
         * to the stored board.
         */
        old = *board;
        do_move(board, move.pos, c);
        result = alphabeta(board, !c, -FLT_MAX, -best_move.score, depth, n);
        *board = old;

        /* Score from alphabeta will be for the opponent. */
        score = -result.score;

        /* If move leads to guaranteed win, return it. */
        if (result.end && score > 0) {
            fprintf(stderr, "score %.2f (guaranteed win)\n", score);

            best_move.pos = move.pos;
            best_move.score = score;
            best_move.end = 1;
            return best_move;
        }

        /*
         * If this move is the best one found yet, print score and update
         * best_move. Otherwise just print newline and go to next.
         */
        if (score > best_move.score) {
            fprintf(stderr, "score %.2f\n", score);

            best_move.pos = move.pos;
            best_move.score = score;
            best_move.end = result.end;
        } else {
            fprintf(stderr, "\n");
        }
    }

    /* Free the array of moves allocated by get_scored_moves(). */
    free(moves);

    return best_move;
}

/**
 * Endgame search:
 * Like ab_search, but search until the end of the game always and limit to
 * win-loss-draw. This gives the (-1, 1) window for alphabeta.
 */
move_score_t endgame_search(board_t *board, int c, long *n) {
    move_score_t *moves;
    board_t old;
    size_t n_moves;
    float score, best_score;
    move_score_t move, result;
    int ii;

    /* Get available moves, sorted by eval score. */
    get_scored_moves(&moves, &n_moves, board, c);

    best_score = -1;

    /* If there are no available moves, pass. */
    if (n_moves == 0) {
        move.pos = -1;
        return move;
    }

    for (ii = 0; ii < n_moves; ++ii) {
        move = moves[ii];

        fprintf(stderr, "Scanning move %2d ... ", move.pos);

        /*
         * Store old board, apply the move, evaluate with alphabeta, then revert
         * to the stored board.
         */
        old = *board;
        do_move(board, move.pos, c);
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


/**
 * Alphabeta:
 * Run the alphabeta minimax algorithm to recursively evaluate a board to a
 * specified depth. Returns a move_score_t containing the best move, its score,
 * and whether the game finished. Parameter n is used to track total number of
 * nodes visited.
 */
move_score_t alphabeta(board_t *board, int c, float alpha, float beta, int depth, long *n) {
    uint64_t moves, old_b, old_w;
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
 * Get scored moves:
 * Gets all the moves available to color c for board b, puts them in an array,
 * and sorts them. The resulting array is at *move_arr. The number of moves is
 * put into *n. *move_arr must be freed later.
 */
void get_scored_moves(move_score_t **move_arr, size_t *n, board_t *board, int c) {
    int ii, move;
    uint64_t old_b, old_w;
    uint64_t moves_mask = get_moves(board, c);

    /* Get count of moves. */
    *n = popcount(moves_mask);

    /* Allocate space for array of moves. */
    *move_arr = malloc(*n * sizeof(move_score_t));
    if (*move_arr == NULL) {
        fprintf(stderr, "Error: unable to allocate memory for move array.");
        exit(1);
    }

    /* Loop through all moves. */
    for (ii = 0; ii < *n; ++ii) {
        /*
         * Find next move by counting trailing zeros in the mask, then zero out
         * the LSB to remove it from the mask.
         */
        move = __builtin_ctzll(moves_mask);
        moves_mask &= moves_mask - 1;

        /* Add it to the array. */
        (*move_arr)[ii].pos = move;

        /* Store old board. */
        old_b = board->b;
        old_w = board->w;

        /* Apply the move, score it, and put that in the array element. */
        do_move(board, move, c);
        (*move_arr)[ii].score = table_eval(board, c);

        /* Restore the old board. */
        board->b = old_b;
        board->w = old_w;
    }

    /* Finally sort the array using quicksort. */
    qsort(*move_arr, *n, sizeof(move_score_t), compare_moves);
}

/* Comparison function for sorting scored moves. */
int compare_moves(const void *m1, const void *m2) {
    float s1 = ((move_score_t *)m1)->score;
    float s2 = ((move_score_t *)m2)->score;
    return (int)(s2 - s1);
}
