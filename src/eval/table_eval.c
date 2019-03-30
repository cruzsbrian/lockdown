#include "table_eval.h"

#include <stdint.h>
#include "../board/board.h"


float piece_score(uint64_t pieces);

int n_corners(uint64_t pieces);
int n_edges(uint64_t pieces);
int n_other(uint64_t pieces);


const uint64_t m_corner = 0x8100000000000081;
const uint64_t m_edge   = 0x7e8181818181817e;
const uint64_t m_other  = 0x007e7e7e7e7e7e00;


const float w_corner        = 20.;
const float w_edge          = 6.;
const float w_other         = 1.;
const float w_mobility      = 4.;
const float w_flippable     = 0.5;
const float w_stability     = 5.;




float table_eval(board_t *b, int c) {
    uint64_t own, opp, own_moves, opp_moves;

    float score = 0.;

    if (c == BLACK) {
        own = b->b;
        opp = b->w;
    } else {
        own = b->w;
        opp = b->b;
    }

    own_moves = get_moves(b, c);
    opp_moves = get_moves(b, !c);

    score += (piece_score(own) - piece_score(opp));
    score += (piece_score(own_moves) - piece_score(opp_moves)) * w_mobility;

    return score;
}


int endgame_eval(board_t *b, int c) {
    if (c == BLACK) {
        return popcount(b->b) - popcount(b->w);
    } else {
        return popcount(b->w) - popcount(b->b);
    }
}




float piece_score(uint64_t pieces) {
    float score = 0;

    score += (float)n_corners(pieces) * w_corner;
    score += (float)n_edges(pieces) * w_edge;
    score += (float)n_other(pieces) * w_other;

    return score;
}




int n_corners(uint64_t pieces) {
    int result = 0;

    if (pieces & 0x8000000000000000) result++;
    if (pieces & 0x0100000000000000) result++;
    if (pieces & 0x0000000000000080) result++;
    if (pieces & 0x0000000000000001) result++;

    return result;
}

int n_edges(uint64_t pieces) {
    return popcount(m_edge & pieces);
}

int n_other(uint64_t pieces) {
    return popcount(m_other & pieces);
}

