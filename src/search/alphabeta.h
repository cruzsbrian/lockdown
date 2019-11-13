#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <stddef.h>
#include <stdint.h>
#include "search.h"
#include "trans_table.h"

move_score_t ab(board_t *board, int c, int16_t alpha, int16_t beta,
                       int depth, int max_depth, node_t *tt, long *n, int use_tt);

move_score_t ab_ordered(board_t *board, int c, int16_t alpha, int16_t beta,
                       int depth, int max_depth, node_t *tt, long *n);

#endif
