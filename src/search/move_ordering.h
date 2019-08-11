#ifndef MOVE_ORDERING_H
#define MOVE_ORDERING_H

#include <stddef.h>
#include "search.h"
#include "trans_table.h"

void get_scored_moves(move_score_t **move_arr, size_t *n, board_t *board, int c,
                      int depth, node_t *tt, long *n_nodes);

#endif
