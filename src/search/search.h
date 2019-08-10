#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>
#include "../board/board.h"

/* Struct so that searches can return both a move and its score. */
typedef struct {
    int pos; /* squared piece was placed */
    int16_t score; /* minimax score */
    int end; /* if leads to endgame */
} move_score_t;

void search_init(void);
void search_free(void);

/* Search game tree for best move for color c. */
int search(board_t *board, int c, int move_num, float time_left);

#endif
