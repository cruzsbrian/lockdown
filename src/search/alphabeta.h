#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <stddef.h>
#include "search.h"

move_score_t alphabeta(
        board_t *board, int c,
        float alpha, float beta,
        int depth, long *n);

#endif
