#ifndef SEARCH_H
#define SEARCH_H

#include "../board/board.h"

/* Struct so that searches can return both a move and its score. */
typedef struct {
    int pos;
    float score;
    board_t *board;
} move_score_t;

#endif
