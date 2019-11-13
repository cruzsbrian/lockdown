#ifndef MOVE_ORDERING_H
#define MOVE_ORDERING_H

#include <stddef.h>
#include "search.h"
#include "trans_table.h"

int16_t find_score(board_t *board, int c, int depth, node_t *tt, long *n_nodes);

#endif
