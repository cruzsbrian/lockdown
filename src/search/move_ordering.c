#include "move_ordering.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include "../board/board.h"
#include "../eval/table_eval.h"
#include "alphabeta.h"
#include "trans_table.h"

int16_t find_score(board_t *board, int c, int depth, node_t *tt, long *n_nodes) {
    node_t tt_entry;

    tt_entry = lookup_score(tt, *board);
    if (tt_entry.depth >= depth) {
        if (tt_entry.color == c) {
            return tt_entry.score;
        } else {
            return -tt_entry.score;
        }
    }

    return -ab(board, !c, -INT16_MAX, INT16_MAX, 0, depth, tt, n_nodes, 0).score;
}
