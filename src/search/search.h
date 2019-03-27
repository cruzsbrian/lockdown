#ifndef SEARCH_H
#define SEARCH_H

#include "../board/board.h"

/* Struct so that searches can return both a move and its score. */
typedef struct {
    int pos; /* squared piece was placed */
    float score; /* minimax score */
    int end; /* if leads to endgame */
    /*board_t after;*/
} move_score_t;

#endif
