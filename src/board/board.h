#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#define BLACK 0
#define WHITE 1


typedef struct {
    uint64_t b;
    uint64_t w;
} board_t;


/* Board creation */
board_t *create_board(void);
board_t *copy_board(board_t *old);


/* Population count */
int popcount(uint64_t x);
int popcount_board(board_t *board);


/* Move generation and make-move */
uint64_t soutOccl(uint64_t gen, uint64_t pro);
uint64_t nortOccl(uint64_t gen, uint64_t pro);
uint64_t eastOccl(uint64_t gen, uint64_t pro);
uint64_t westOccl(uint64_t gen, uint64_t pro);
uint64_t noEaOccl(uint64_t gen, uint64_t pro);
uint64_t soEaOccl(uint64_t gen, uint64_t pro);
uint64_t noWeOccl(uint64_t gen, uint64_t pro);
uint64_t soWeOccl(uint64_t gen, uint64_t pro);

uint64_t get_moves(board_t *board, int c);

void do_move(board_t *board, uint64_t gen, int c);

void add_piece(board_t *board, int pos, int c);


/* Printing boards */
void print_board(board_t *board);
void print_bits(uint64_t x);


#endif
