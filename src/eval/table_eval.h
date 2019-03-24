#ifndef TABLE_EVAL_H
#define TABLE_EVAL_H

#include "../board/board.h"

float table_eval(board_t *b, int c);
int endgame_eval(board_t *b, int c);

#endif
