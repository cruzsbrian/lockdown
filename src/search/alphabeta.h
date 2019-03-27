#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <stddef.h>
#include "search.h"

move_score_t alphabeta(board_t *board, int c, float alpha, float beta, int depth);

void get_scored_moves(move_score_t **move_arr, size_t *n, board_t *b, int c);

int compare_moves(const void *m1, const void *m2);

#endif
