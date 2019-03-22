#include "simple_eval.h"

#include "../board/board.h"

float simple_eval(board_t *b, int c) {
    if (c == BLACK) {
        return (float)(popcount(b->b) - popcount(b->w));
    } else {
        return (float)(popcount(b->w) - popcount(b->b));
    }
}
