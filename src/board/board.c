#include <stdio.h>
#include <stdlib.h>

#include "board.h"


/**
 * ========================================
 *              Board Creation
 * ========================================
 */

board_t *create_board(void) {
    board_t *new_board = (board_t *)malloc(sizeof(board_t));
    
    if (new_board == NULL) {
        fprintf(stderr, "Error: Unable to allocate new board.");
        exit(1);
    }

    return new_board;
}

board_t *copy_board(board_t *old) {
    board_t *new_board = create_board();
    new_board->b = old->b;
    new_board->w = old->w;
    return new_board;
}




/**
 * ========================================
 *            Population Count
 * ========================================
 */

int popcount(uint64_t x) {
    const uint64_t m1  = 0x5555555555555555;
    const uint64_t m2  = 0x3333333333333333;
    const uint64_t m4  = 0x0f0f0f0f0f0f0f0f;
    const uint64_t h01 = 0x0101010101010101;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    return (x * h01) >> 56;
}

int popcount_board(board_t *board) {
    return popcount(board->b) + popcount(board->w);
}




/**
 * ========================================
 *       Move Generation and Make-Move
 * ========================================
 */

/* Masks to filter out end files. */
const uint64_t notAFile = 0xfefefefefefefefe;
const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f;

/*
 * Fill algorithms:
 * Smears the gen bitmap in the specified direction as long as there is
 * something in the pro bitmap. For east, west, and diagonals, we exclude the 
 * first file in that shift direction to avoid wrap-around.
 *
 * The result takes each piece in gen, and draws a ray from it in the specified
 * direction as far as pieces in pro go in that direction.
 */
uint64_t soutOccl(uint64_t gen, uint64_t pro) {
    gen |= pro & (gen >> 8);
    pro &=       (pro >> 8);
    gen |= pro & (gen >> 16);
    pro &=       (pro >> 16);
    gen |= pro & (gen >> 32);
    return gen;
}

uint64_t nortOccl(uint64_t gen, uint64_t pro) {
    gen |= pro & (gen << 8);
    pro &=       (pro << 8);
    gen |= pro & (gen << 16);
    pro &=       (pro << 16);
    gen |= pro & (gen << 32);
    return gen;
}

uint64_t eastOccl(uint64_t gen, uint64_t pro) {
    pro &= notAFile;
    gen |= pro & (gen << 1);
    pro &=       (pro << 1);
    gen |= pro & (gen << 2);
    pro &=       (pro << 2);
    gen |= pro & (gen << 4);
    return gen;
}

uint64_t westOccl(uint64_t gen, uint64_t pro) {
    pro &= notHFile;
    gen |= pro & (gen >> 1);
    pro &=       (pro >> 1);
    gen |= pro & (gen >> 2);
    pro &=       (pro >> 2);
    gen |= pro & (gen >> 4);
    return gen;
}

uint64_t noEaOccl(uint64_t gen, uint64_t pro) {
    pro &= notAFile;
    gen |= pro & (gen << 9);
    pro &=       (pro << 9);
    gen |= pro & (gen << 18);
    pro &=       (pro << 18);
    gen |= pro & (gen << 36);
    return gen;
}

uint64_t soEaOccl(uint64_t gen, uint64_t pro) {
    pro &= notAFile;
    gen |= pro & (gen >> 7);
    pro &=       (pro >> 7);
    gen |= pro & (gen >> 14);
    pro &=       (pro >> 14);
    gen |= pro & (gen >> 28);
    return gen;
}

uint64_t noWeOccl(uint64_t gen, uint64_t pro) {
    pro &= notHFile;
    gen |= pro & (gen << 7);
    pro &=       (pro << 7);
    gen |= pro & (gen << 14);
    pro &=       (pro << 14);
    gen |= pro & (gen << 28);
    return gen;
}

uint64_t soWeOccl(uint64_t gen, uint64_t pro) {
    pro &= notHFile;
    gen |= pro & (gen >> 9);
    pro &=       (pro >> 9);
    gen |= pro & (gen >> 18);
    pro &=       (pro >> 18);
    gen |= pro & (gen >> 36);
    return gen;
}


/*
 * Move generation:
 * Use own pieces as generator, opponents pieces as propogator. After rays have
 * been calculated, they must be &-ed with pro to exclude the generator piece.
 * Then shift one more step in the ray direction (making sure not to wrap
 * around), and & with empty squares to get playable squares.
 *
 * Returns a long representing squares that can be played in.
 */
uint64_t moves(board_t *board, int c) {
    uint64_t gen, pro, empty, tmp, moves;

    if (c == BLACK) {
        gen = board->b;
        pro = board->w;
    } else {
        gen = board->w;
        pro = board->b;
    }

    moves = 0L;
    empty = ~(gen | pro);

    tmp = soutOccl(gen, pro) & pro;
    moves |= (tmp >> 8) & empty;

    tmp = nortOccl(gen, pro) & pro;
    moves |= (tmp << 8) & empty;

    tmp = eastOccl(gen, pro) & pro;
    moves |= (tmp << 1) & notAFile & empty;

    tmp = westOccl(gen, pro) & pro;
    moves |= (tmp >> 1) & notHFile & empty;

    tmp = noEaOccl(gen, pro) & pro;
    moves |= (tmp << 9) & notAFile & empty;

    tmp = soEaOccl(gen, pro) & pro;
    moves |= (tmp >> 7) & notAFile & empty;

    tmp = noWeOccl(gen, pro) & pro;
    moves |= (tmp << 7) & notHFile & empty;

    tmp = soWeOccl(gen, pro) & pro;
    moves |= (tmp >> 9) & notHFile & empty;

    return moves;
}




/*
 * Make-move:
 * Gen is a single-bit long representing the added piece. Filling from gen along
 * opponent pieces and &-ing with rays in the opposite direction from existing
 * pieces gives only lines of opponent pieces that have the new piece on one
 * side and an existing own piece on the other side.
 */
void do_move(board_t *board, uint64_t gen, int c) {
    uint64_t own, pro, diff;

    if (c == BLACK) {
        own = board->b;
        pro = board->w;

        board->b |= gen;
    } else {
        own = board->w;
        pro = board->b;

        board->w |= gen;
    }
    
    diff = 0L;
    diff |= soutOccl(gen, pro) & nortOccl(own, pro);
    diff |= nortOccl(gen, pro) & soutOccl(own, pro);
    diff |= eastOccl(gen, pro) & westOccl(own, pro);
    diff |= westOccl(gen, pro) & eastOccl(own, pro);
    diff |= noEaOccl(gen, pro) & soWeOccl(own, pro);
    diff |= soEaOccl(gen, pro) & noWeOccl(own, pro);
    diff |= noWeOccl(gen, pro) & soEaOccl(own, pro);
    diff |= soWeOccl(gen, pro) & noEaOccl(own, pro);

    board->b ^= diff;
    board->w ^= diff;
}

void add_piece(board_t *board, int pos, int c) {
    if (c == BLACK) {
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
    printf("\n");
}

void print_bits(uint64_t x) {
    int ii;
    for (ii = 0; ii < 64; ++ii) {
        printf((x >> ii) & 1L ? "1 " : "_ ");
        if (ii % 8 == 7) {
            printf("\n");
        }
    }
    printf("\n");
}
