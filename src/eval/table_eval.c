#include "table_eval.h"

#include <stdint.h>
#include "../board/board.h"

const uint64_t m_corner = 0x8100000000000081;
const uint64_t m_edge   = 0x7e8181818181817e;
const uint64_t m_other  = 0x007e7e7e7e7e7e00;

float table_eval(board_t *b, int c) {
    const float corner        = 11.;
    const float edge          = 6.;
    const float other         = 2.;
    const float mobility      = 4.;

    float score = 0.;

    uint64_t own, opp;
    if (c == BLACK) {
        own = b->b;
        opp = b->w;
    } else {
        own = b->w;
        opp = b->b;
    }

    if (popcount_board(b) < 50) {
        score += (float)(popcount(m_corner & own) - popcount(m_corner & opp)) * corner;
        score += (float)(popcount(m_edge & own) - popcount(m_edge & opp)) * edge;
        score += (float)(popcount(m_other & own) - popcount(m_other & opp)) * other;

        score += (float)(popcount(get_moves(b, BLACK)) - popcount(get_moves(b, WHITE))) * mobility;
    } else {
        score = (float)endgame_eval(b, c);
    }

    return score;
}

int endgame_eval(board_t *b, int c) {
    if (c == BLACK) {
        return popcount(b->b) - popcount(b->w);
    } else {
        return popcount(b->w) - popcount(b->b);
    }
}
