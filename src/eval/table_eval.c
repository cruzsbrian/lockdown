#include "table_eval.h"

#include <stdint.h>
#include "../board/board.h"


static inline int16_t piece_score(uint64_t pieces, const int16_t *w);

static inline int n_corners(uint64_t pieces);
static inline int x_square(uint64_t pieces);
static inline int n_edges(uint64_t pieces);
static inline int n_other(uint64_t pieces);


/* Masks for getting sets of pieces. */
const uint64_t m_corner = 0x8100000000000081;
const uint64_t m_edge   = 0x3c7e818181817e3c;
const uint64_t m_other  = 0x003c7e7e7e7e3c00;


/* Weights for different score metrics, broken up into 4 game phases. */
enum w_index { CORNER, EDGE, MOBILITY, FLIPPABLE, FRONTIER, X_SQUARE };
const int16_t weights[4][6] = {{ 85, 10, 20, 0, -25, -65 },
                               { 85, 10, 20, 0, -25, -65 },
                               { 65, 5, 15, 1, -15, -55 },
                               { 55, 5, 5, 2, -5, -25 }};




/**
 * Table-based eval:
 * Calculates score for the board based on material, mobility, number of pieces
 * that can be flipped in the next move, fontier pieces, and x-square pieces.
 */
int16_t table_eval(board_t *b, int c) {
    uint64_t own, opp,
             own_moves, opp_moves,
             own_flip, opp_flip;
    int game_phase;

    int16_t score = 0;

    if (c == BLACK) {
        own = b->b;
        opp = b->w;
    } else {
        own = b->w;
        opp = b->b;
    }

    game_phase = (popcount(own) + popcount(opp) - 4) / 15;

    get_moves_flips(&own_moves, &own_flip, b, c);
    get_moves_flips(&opp_moves, &opp_flip, b, !c);

    score += piece_score(own, weights[game_phase]) - piece_score(opp, weights[game_phase]);
    score += (popcount(own_moves) - popcount(opp_moves)) * weights[game_phase][2];
    score += (popcount(own_flip) - popcount(opp_flip)) * weights[game_phase][3];
    score += (get_frontier(b, c) - get_frontier(b, !c)) * weights[game_phase][4];
    score += (x_square(own) - x_square(opp)) * weights[game_phase][5];

    return score;
}


/**
 * Faster eval for endgame, we only care about final score.
 */
int endgame_eval(board_t *b, int c) {
    if (c == BLACK) {
        return popcount(b->b) - popcount(b->w);
    } else {
        return popcount(b->w) - popcount(b->b);
    }
}




/**
 * Find the weighted score for a set of pieces with weights w
 */
int16_t piece_score(uint64_t pieces, const int16_t *w) {
    int16_t score = 0;

    score += (int16_t)n_corners(pieces) * w[0];
    score += (int16_t)n_edges(pieces) * w[1];

    return score;
}




/*
 * Count corners in a set of pieces.
 */
static inline int n_corners(uint64_t pieces) {
    int result = 0;

    if (pieces & 0x8000000000000000) result++;
    if (pieces & 0x0100000000000000) result++;
    if (pieces & 0x0000000000000080) result++;
    if (pieces & 0x0000000000000001) result++;

    return result;
}

static inline int x_square(uint64_t pieces) {
    int result = 0;

    if (pieces & 0x0040000000000000 && !(pieces & 0x8000000000000000)) result++;
    if (pieces & 0x0002000000000000 && !(pieces & 0x0100000000000000)) result++;
    if (pieces & 0x0000000000004000 && !(pieces & 0x0000000000000080)) result++;
    if (pieces & 0x0000000000000200 && !(pieces & 0x0000000000000001)) result++;

    return result;
}

/* 
 * Count non-corner edges in a set of pieces.
 */
static inline int n_edges(uint64_t pieces) {
    return popcount(m_edge & pieces);
}

/* 
 * Count non-edges, non-corners in a set of pieces.
 */
static inline int n_other(uint64_t pieces) {
    return popcount(m_other & pieces);
}

