#ifndef TABLE_EVAL_H
#define TABLE_EVAL_H

#include <stdint.h>
#include "../board/board.h"

int16_t table_eval(board_t *b, int c);
int endgame_eval(board_t *b, int c);

int n_corners(uint64_t pieces);
int x_square(uint64_t pieces);
int c_square(uint64_t pieces);
int n_edges(uint64_t pieces);
int n_other(uint64_t pieces);

#endif
