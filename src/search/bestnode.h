#ifndef BESTNODE_H
#define BESTNODE_H

#include <stdint.h>
#include "trans_table.h"

int8_t bns(board_t *board_t, int c, int depth, node_t *tt, long *n);

#endif
