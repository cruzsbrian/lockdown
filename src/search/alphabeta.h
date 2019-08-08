#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <stddef.h>
#include "search.h"
#include "trans_table.h"

move_score_t alphabeta(board_t *board, int c, float alpha, float beta,
                       int depth, int max_depth, node_t *tt, long *n, int use_tt);

#endif
