#ifndef NEGAMAX_H
#define NEGAMAX_H

#include <stdint.h>

#include "../board/board.h"

/* Struct so that negamax can return both a move and its score. */
typedef struct {
    int pos;
    float score;
} move_score_t;

move_score_t negamax(board_t *board, int c, int depth);
move_score_t endgame(board_t *board, int c);

#endif
