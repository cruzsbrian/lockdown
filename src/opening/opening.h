#ifndef OPENING_H
#define OPENING_H

#include <stdint.h>
#include "../board/board.h"

typedef struct {
    uint64_t taken;
    uint64_t black;
    int8_t pos;
} opening_t;

opening_t* init_opening_book(void);
void free_opening_book(opening_t *ob);

int8_t search_book(opening_t *ob, board_t *board);

#endif
