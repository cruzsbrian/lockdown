#include <stdio.h>
#include <stdlib.h>

#include "board.h"


board_t *create_board(void) {
    /* Use calloc to zero out memory */
    board_t *new_board = (board_t *)calloc(1, sizeof(board_t));
    
    if (new_board == NULL) {
        fprintf(stderr, "Error: Unable to allocate new board.");
        exit(1);
    }

    return new_board;
}




void add_piece(board_t *board, int pos, int color) {
    if (color == BLACK) {
        board->b |= (1L << pos);
    } else {
        board->w |= (1L << pos);
    }
}




void print_board(board_t *board) {
    int pos, b, w;

    int ii, jj;
    for (ii = 0; ii < 8; ++ii) {
        for (jj = 0; jj < 8; ++jj) {
            pos = (ii * 8) + jj;
            b = (board->b >> pos) & 1L;
            w = (board->w >> pos) & 1L;

            if (w) {
                printf("W ");
            } else if (b) {
                printf("B ");
            } else {
                printf("_ ");
            }
        }
        printf("\n");
    }
}
