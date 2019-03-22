#ifndef NEGAMAX_H
#define NEGAMAX_H

#include <stdint.h>

#include "../board/board.h"

typedef struct {
    int pos;
    float score;
} move_score_t;

move_score_t negamax(board_t *board, int c, int depth);

#endif
