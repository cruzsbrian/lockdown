#include "table_eval.h"

#include <stdint.h>
#include "../board/board.h"


int16_t piece_score(uint64_t pieces);

int n_corners(uint64_t pieces);
int n_edges(uint64_t pieces);
int n_other(uint64_t pieces);


/* Masks for getting sets of pieces. */
const uint64_t m_corner = 0x8100000000000081;
const uint64_t m_edge   = 0x7e8181818181817e;
const uint64_t m_other  = 0x007e7e7e7e7e7e00;


/* Weights for different score metrics. */
const int16_t w_corner     = 40;
const int16_t w_edge       = 10;
const int16_t w_other      = 1;
const int16_t w_mobility   = 5;
const int16_t w_flippable  = 1;




/**
 * Table-based eval:
 * Calculates score for the board based on a piece-score matrix, mobility, and
 * number of pieces that can be flipped in the next move. Mobility and flipping
 * are also weighted by the same matrix.
 */
int16_t table_eval(board_t *b, int c) {
    uint64_t own, opp,
             own_moves, opp_moves,
             own_flip, opp_flip;

    int16_t score = 0;

    if (c == BLACK) {
        own = b->b;
        opp = b->w;
    } else {
        own = b->w;
        opp = b->b;
    }

    get_moves_flips(&own_moves, &own_flip, b, c);
    get_moves_flips(&opp_moves, &opp_flip, b, !c);

    score += piece_score(own) - piece_score(opp);
    score += (piece_score(own_moves) - piece_score(opp_moves)) * w_mobility;
    score += (piece_score(own_flip) - piece_score(opp_flip)) * w_flippable;

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
 * Find the weighted score for a set of pieces
 */
int16_t piece_score(uint64_t pieces) {
    int16_t score = 0;

    score += (int16_t)n_corners(pieces) * w_corner;
    score += (int16_t)n_edges(pieces) * w_edge;
    score += (int16_t)n_other(pieces) * w_other;

    return score;
}




/*
 * Count corners in a set of pieces.
 */
int n_corners(uint64_t pieces) {
    int result = 0;

    if (pieces & 0x8000000000000000) result++;
    if (pieces & 0x0100000000000000) result++;
    if (pieces & 0x0000000000000080) result++;
    if (pieces & 0x0000000000000001) result++;

    return result;
}

/* 
 * Count non-corner edges in a set of pieces.
 */
int n_edges(uint64_t pieces) {
    return popcount(m_edge & pieces);
}

/* 
 * Count non-edges, non-corners in a set of pieces.
 */
int n_other(uint64_t pieces) {
    return popcount(m_other & pieces);
}

