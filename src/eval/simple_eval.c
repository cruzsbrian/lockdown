#include "simple_eval.h"

#include "../board/board.h"

float simple_eval(board_t *b, int c) {
    int material, mobility;

    if (c == BLACK) {
        material =  popcount(b->b) - popcount(b->w);
    } else {
        material =  popcount(b->w) - popcount(b->b);
    }

    mobility = popcount(get_moves(b, c));

    return (float)(material + mobility);
}
