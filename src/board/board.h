#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#define BLACK 0
#define WHITE 1


typedef struct {
    uint64_t b;
    uint64_t w;
} board_t;


board_t *create_board(void);
board_t *copy_board(board_t *old);


int popcount(uint64_t bits);
int popcount_board(board_t *board);


void add_piece(board_t *board, int pos, int color);


void print_board(board_t *board);


#endif
