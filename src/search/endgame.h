#ifndef ENDGAME_H
#define ENDGAME_H

#include <stdint.h>
#include "../board/board.h"
#include "search.h"
#include "trans_table.h"

int endgame_search(board_t *board, int c, node_t *tt, long *n);
move_score_t ab_ff(board_t *board, int c, int16_t alpha, int16_t beta, int depth,
                    int max_depth, node_t *tt, long *n);

#endif
