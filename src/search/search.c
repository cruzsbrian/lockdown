#include "search.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"
#include "../opening/opening.h"
#include "alphabeta.h"
#include "bestnode.h"
#include "move_ordering.h"
#include "trans_table.h"


const int ENDGAME_MOVES = 18;
const int MAX_SEARCH_DEPTH = 15;
const float ENDGAME_TIME = 10.;
const float EARLY_MOVE_BIAS = 1.3;
const float MIN_SEARCH_TIME = 0.1;


int iter_ab_search(board_t *board, int c, int step, int max_depth, float max_time, long *n);
int iter_bns(board_t *board, int c, int step, int max_depth, float max_time, long *n);
move_score_t ab_search(board_t *board, int c, int depth, long *n);

int endgame_search(board_t *board, int c, long *n);

float get_time_budget(int move_num, float time_left);


node_t *trans_table;
opening_t *opening_book;

int on_opening_book = 1;


void search_init() {
    init_hash();
    trans_table = init_trans_table();
    opening_book = init_opening_book();
}

void search_free() {
    free_trans_table(trans_table);
    free_opening_book(opening_book);
}


/**
 * Top-level search function:
 * Decides whether to call iter_ab_search or endgame_search, times the search,
 * and prints debugging info.
 */
int search(board_t *board, int c, int move_num, float time_left) {
    int result;
    long n_nodes = 0L;
    clock_t start, end;
    float time_budget, seconds, nps;

    fprintf(stderr, "%.2f seconds left\n", time_left);
    time_budget = get_time_budget(move_num, time_left);

    start = clock();

    if (move_num >= 60 - ENDGAME_MOVES) {
        fprintf(stderr, "Running end-game solver.\n");
        result = endgame_search(board, c, &n_nodes);
    } else {
        result = -1;

        if (on_opening_book) {
            result = search_book(opening_book, board);
            fprintf(stderr, "Opening book result: %d\n", result);
        }

        if (result == -1) {
            on_opening_book = 0;

            fprintf(stderr, "Running alphabeta search with %.2f seconds.\n",
                    time_budget);
            /* result = iter_bns(board, c, 1, 60 - move_num, time_budget, &n_nodes); */
            result = iter_ab_search(board, c, 1, MAX_SEARCH_DEPTH, time_budget, &n_nodes);
        }
    }

    end = clock();

    /* Convert search time to seconds, find nodes per second, print. */
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    nps = (float)n_nodes / seconds;
    fprintf(stderr, "%ld nodes in %.2fs @ %.0f node/s\n",
            n_nodes, seconds, nps);

    return result;
}


int iter_bns(board_t *board, int c, int step, int max_depth, float max_time, long *n) {
    int depth;
    int best_move;
    float last_time, total_time, time_factor;

    best_move = -1;
    last_time = 0.;
    total_time = 0.;
    time_factor = 4;   /* Initial guess based on previous games. */
    depth = 1;

    while (total_time + last_time * time_factor <= max_time && depth <= max_depth) {
        clock_t start, end;
        float seconds;
        start = clock();

        best_move = (int) bns(board, c, depth, trans_table, n);

        end = clock();
        seconds = (float)(end - start) / CLOCKS_PER_SEC;

        /*
         * Don't use branching factor data from below depth 7, since the timing
         * isn't accurate to how long deeper searches will take.
         */
        if (depth > 7) {
            time_factor = (time_factor + (seconds / last_time)) / 2;
        }

        last_time = seconds;

        fprintf(stderr, "Depth %2d best move %2d (time %.2e)\n",
                depth, best_move, seconds);

        depth++;
    }

    return best_move;
}


/**
 * Iterative deepening alphabeta search:
 * Repeatedly calls ab_search with greater depth each time, until the next
 * search is estimated to take too long. Estimates the time for the next search
 * based on the previous search and an estimated branching factor from previous
 * searches. If the time already taken plus the estimated time is more than
 * max_time, exit the loop and return the result from the latest search.
 */
int iter_ab_search(board_t *board, int c, int step, int max_depth, float max_time, long *n) {
    int depth;
    move_score_t best_move;
    float last_time, total_time, time_factor;

    best_move.pos = -1;
    last_time = 0.;
    total_time = 0.;
    time_factor = 4;   /* Initial guess based on previous games. */
    depth = 1;

    while (total_time + last_time * time_factor <= max_time && depth <= max_depth) {
        clock_t start, end;
        float seconds;
        start = clock();

        best_move = ab_search(board, c, depth, n);

        end = clock();
        seconds = (float)(end - start) / CLOCKS_PER_SEC;

        /*
         * Don't use branching factor data from below depth 7, since the timing
         * isn't accurate to how long deeper searches will take.
         */
        if (depth > 7) {
            time_factor = (time_factor + (seconds / last_time)) / 2;
        }

        last_time = seconds;

        fprintf(stderr, "Depth %2d best move %2d (score %3d, time %.2e)\n",
                depth, best_move.pos, best_move.score, seconds);

        depth++;
    }

    return best_move.pos;
}


/*
 * Alphabeta search:
 * Orders moves and then runs an alphabeta search to a given depth on each one
 * to determine it's score. Returns the best move along with it's score.
 */
move_score_t ab_search(board_t *board, int c, int depth, long *n) {
    board_t old;
    move_score_t *moves;
    size_t n_moves;

    int16_t score;
    int8_t move;
    move_score_t best_move, result;

    int ii;

    /* Get available moves, sorted by eval score. */
    get_scored_moves(&moves, &n_moves, board, c, 5, trans_table, n);

    /* Start with minimum score */
    best_move.score = -INT16_MAX;
    best_move.pos = -1;

    /* If there are no available moves, return a pass. */
    if (n_moves == 0) {
        return best_move;
    }

    /* Search each move. */
    for (ii = 0; ii < n_moves; ++ii) {
        move = moves[ii].pos;

        /*
         * Store old board, apply the move, evaluate with alphabeta, then revert
         * to the stored board.
         */
        old = *board;
        do_move(board, move, c);
        result = alphabeta(board, !c, -INT16_MAX, -best_move.score, 0, depth, trans_table, n, 1);
        *board = old;

        /* Score from alphabeta will be for the opponent. */
        score = -result.score;

        /*
         * If this move is the best one found yet, print score and update
         * best_move. Otherwise just print newline and go to next.
         */
        if (score > best_move.score) {
            best_move.pos = move;
            best_move.score = score;
            best_move.end = result.end;
        }
    }

    /* Free the array of moves allocated by get_scored_moves(). */
    free(moves);

    return best_move;
}


/**
 * Endgame search:
 * Runs alphabeta search on each move, but with a window of [-1, 1], restricting
 * results to win-loss-draw.
 */
int endgame_search(board_t *board, int c, long *n) {
    board_t old;
    move_score_t *moves;
    size_t n_moves;
    int16_t best_score, score;
    int8_t move;
    move_score_t result;
    int ii;

    /* Get available moves, sorted by eval score. */
    get_scored_moves(&moves, &n_moves, board, c, 5, trans_table, n);

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
        result = alphabeta(board, !c, -1, -best_score, 0, 60, trans_table, n, 0);
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

    return moves[0].pos;
}


/**
 * Get time budget:
 * Calculates budgeted time for searching the next move based on move number and
 * time left. Leaves a set amount of time for the endgame search, budgets a
 * minimal amount of time for the first move for black, and distributes the rest
 * of the time evenly-ish, with preference toward earlier moves where the game
 * tree is bigger.
 */
float get_time_budget(int move_num, float time_left) {
    int moves_left;
    float avg_move_time, result;

    /* The first move for black doesn't matter. */
    if (move_num == 0) {
        return 0.01;
    }

    /* Save 40s for the endgame solver. */
    time_left -= ENDGAME_TIME;

    /*
     * Find moves the bot has left to play, not including endgame moves. Note
     * that move_num counts bot's moves and opponent's moves.
     */
    moves_left = (60 - ENDGAME_MOVES - move_num) / 2 + 1;

    avg_move_time = time_left / (float)moves_left;
    
    /*
     * If there are a number of moves left, bias toward early moves. This is
     * mostly offset by searches ending earlier than the time budgeted. Also we
     * really don't want to do this right near endgame so as not to cut into
     * endgame solver time.
     */
    if (moves_left > 2) {
        result = avg_move_time * EARLY_MOVE_BIAS;
    } else {
        result = avg_move_time;
    }

    /* Minimum time in case we go over time and time_left is too small. */
    if (result < MIN_SEARCH_TIME) return MIN_SEARCH_TIME;

    return result;
}
