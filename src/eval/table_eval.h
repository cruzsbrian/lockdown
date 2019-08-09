#ifndef TABLE_EVAL_H
#define TABLE_EVAL_H

#include <stdint.h>
#include "../board/board.h"

int16_t table_eval(board_t *b, int c);
int endgame_eval(board_t *b, int c);

#endif
