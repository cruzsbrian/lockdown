#include "table_eval.h"

#include <stdio.h>
#include <stdint.h>
#include "../board/board.h"


int16_t piece_score(uint64_t pieces, const int16_t *w);

/* Masks for getting sets of pieces. */
const uint64_t m_corner   = 0x8100000000000081;
const uint64_t m_c_square = 0x4281000000004281;
const uint64_t m_edge     = 0x3c7e818181817e3c;
const uint64_t m_other    = 0x003c7e7e7e7e3c00;


/*
 * Weights for different score metrics, broken up into 4 game phases.
 * corners, edges, mobility, flippable pieces, frontier pieces, x-square, c-square, stable pieces.
 */
/* const int16_t weights[9][7] = { */
/*     { 916, -27, 21,  52, -73, -469, -146 }, */
/*     { 869, -8,  29,  52, -79, -405, -127 }, */
/*     { 801, 5,   37,  48, -83, -346, -103 }, */
/*     { 721, 13,  48,  42, -84, -293, -75 }, */
/*     { 631, 17,  61,  35, -81, -243, -48 }, */
/*     { 535, 19,  80,  27, -72, -195, -22 }, */
/*     { 425, 21,  108, 18, -54, -146, 4 }, */
/*     { 295, 25,  168, 8,  -17, -85,  29 }, */
/*     { 159, 39,  283, 1,  -5,  -30,  44 } */
/* }; */
const int16_t weights[8][8] = {
    { 464, -7, 30, 54, -81, -439, -136, 464 },
    { 319, 5, 39, 49, -86, -376, -109, 539 },
    { 398, 14, 50, 43, -87, -319, -80, 373 },
    { 456, 19, 64, 36, -84, -262, -53, 224 },
    { 426, 21, 83, 28, -75, -206, -23, 147 },
    { 353, 22, 111, 19, -55, -148, 4, 93 },
    { 228, 23, 162, 10, -14, -86, 23, 58 },
    { 103, 22, 267, 2, 17, -24, 26, 39 }
};
const int game_phase[12] = {0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7};




/**
 * Table-based eval:
 * Calculates score for the board based on material, mobility, number of pieces
 * that can be flipped in the next move, frontier pieces, and x-square and
 * c-square pieces.
 */
int16_t table_eval(board_t *b, int c) {
    uint64_t own, opp,
             own_moves, opp_moves,
             own_flip, opp_flip;
    int move_num, gp;

    int16_t score = 0;

    if (c == BLACK) {
        own = b->b;
        opp = b->w;
    } else {
        own = b->w;
        opp = b->b;
    }

    move_num = popcount(own) + popcount(opp) - 5;
    gp = game_phase[move_num / 5];

    get_moves_flips(&own_moves, &own_flip, b, c);
    get_moves_flips(&opp_moves, &opp_flip, b, !c);

    score += piece_score(own, weights[gp]) - piece_score(opp, weights[gp]);
    score += (popcount(own_moves) - popcount(opp_moves)) * weights[gp][2];
    score += (popcount(own_flip) - popcount(opp_flip)) * weights[gp][3];
    score += (get_frontier(b, c) - get_frontier(b, !c)) * weights[gp][4];
    score += (x_square(own) - x_square(opp)) * weights[gp][5];
    score += (c_square(own) - c_square(opp)) * weights[gp][6];
    score += get_stable(b, c) * weights[gp][7];

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
int n_corners(uint64_t pieces) {
    int result = 0;

    if (pieces & 0x8000000000000000) result++;
    if (pieces & 0x0100000000000000) result++;
    if (pieces & 0x0000000000000080) result++;
    if (pieces & 0x0000000000000001) result++;

    return result;
}

int x_square(uint64_t pieces) {
    int result = 0;

    if (pieces & 0x0040000000000000 && !(pieces & 0x8000000000000000)) result++;
    if (pieces & 0x0002000000000000 && !(pieces & 0x0100000000000000)) result++;
    if (pieces & 0x0000000000004000 && !(pieces & 0x0000000000000080)) result++;
    if (pieces & 0x0000000000000200 && !(pieces & 0x0000000000000001)) result++;

    return result;
}

int c_square(uint64_t pieces) {
    return popcount(m_c_square & pieces);
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

