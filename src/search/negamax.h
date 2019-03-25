#ifndef NEGAMAX_H
#define NEGAMAX_H

#include "search.h"
#include "../board/board.h"

move_score_t negamax(board_t *board, int c, int depth);

#endif
