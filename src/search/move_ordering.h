#ifndef MOVE_ORDERING_H
#define MOVE_ORDERING_H

#include <stddef.h>
#include "search.h"
#include "trans_table.h"

void get_scored_moves(move_score_t **move_arr, size_t *n, board_t *board, int c, trans_table_t *tt);

#endif
