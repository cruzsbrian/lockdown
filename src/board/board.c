#include <stdio.h>
#include <stdlib.h>

#include "board.h"


uint32_t hash_vals[2][64];


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
    new_board->hash = old->hash;
    return new_board;
}




void init_hash() {
    int ii;

    srand(1337);

    for (ii = 0; ii < 64; ++ii) {
        hash_vals[0][ii] = rand();
        hash_vals[1][ii] = rand();
    }
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
 * Shift algorithms:
 * Shifts the pieces one step in the specified direction, making sure pieces
 * don't wrap around.
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

uint64_t soutOne(uint64_t gen) {
    return gen >> 8;
}

uint64_t nortOccl(uint64_t gen, uint64_t pro) {
    gen |= pro & (gen << 8);
    pro &=       (pro << 8);
    gen |= pro & (gen << 16);
    pro &=       (pro << 16);
    gen |= pro & (gen << 32);
    return gen;
}

uint64_t nortOne(uint64_t gen) {
    return gen << 8;
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

uint64_t eastOne(uint64_t gen) {
    return (gen & notAFile) << 1;
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

uint64_t westOne(uint64_t gen) {
    return (gen & notHFile) >> 1;
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

uint64_t noEaOne(uint64_t gen) {
    return (gen & notAFile) << 9;
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

uint64_t soEaOne(uint64_t gen) {
    return (gen & notAFile) >> 7;
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

uint64_t noWeOne(uint64_t gen) {
    return (gen & notHFile) << 7;
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

uint64_t soWeOne(uint64_t gen) {
    return (gen & notHFile) >> 9;
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
uint64_t get_moves(board_t *board, int c) {
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


/**
 * Get moves and flips:
 * Calculate available moves as above, but also keep track of which pieces are
 * flipped by the move. Stores both values in *moves and *flips.
 */
void get_moves_flips(uint64_t *moves, uint64_t *flips, board_t *board, int c) {
    uint64_t gen, pro, empty, tmp, m, f;

    if (c == BLACK) {
        gen = board->b;
        pro = board->w;
    } else {
        gen = board->w;
        pro = board->b;
    }

    m = 0L;
    f = 0L;
    empty = ~(gen | pro);

    tmp = soutOccl(gen, pro) & pro;
    if ((tmp >> 8) & empty) f |= tmp;
    m |= (tmp >> 8) & empty;

    tmp = nortOccl(gen, pro) & pro;
    if ((tmp << 8) & empty) f |= tmp;
    m |= (tmp << 8) & empty;

    tmp = eastOccl(gen, pro) & pro;
    if ((tmp << 1) & notAFile & empty) f |= tmp;
    m |= (tmp << 1) & notAFile & empty;

    tmp = westOccl(gen, pro) & pro;
    if ((tmp >> 1) & notHFile & empty) f |= tmp;
    m |= (tmp >> 1) & notHFile & empty;

    tmp = noEaOccl(gen, pro) & pro;
    if ((tmp << 9) & notAFile & empty) f |= tmp;
    m |= (tmp << 9) & notAFile & empty;

    tmp = soEaOccl(gen, pro) & pro;
    if ((tmp >> 7) & notAFile & empty) f |= tmp;
    m |= (tmp >> 7) & notAFile & empty;

    tmp = noWeOccl(gen, pro) & pro;
    if ((tmp << 7) & notHFile & empty) f |= tmp;
    m |= (tmp << 7) & notHFile & empty;

    tmp = soWeOccl(gen, pro) & pro;
    if ((tmp >> 9) & notHFile & empty) f |= tmp;
    m |= (tmp >> 9) & notHFile & empty;

    *moves = m;
    *flips = f;
}

/**
 * Get frontier pieces:
 * Gives the number of pieces of color c that are adjacent to empty squares.
 * Takes the pieces of color c, and shifts in every direction, &-ing with empty
 * pieces.
 */
int get_frontier(board_t *board, int c) {
    uint64_t own, opp, empty, frontier;

    if (c == BLACK) {
        own = board->b;
        opp = board->w;
    } else {
        own = board->w;
        opp = board->b;
    }

    empty = ~(own | opp);
    frontier = 0L;

    frontier |= (own >> 8) & empty;
    frontier |= (own << 8) & empty;
    frontier |= (own << 1) & notAFile & empty;
    frontier |= (own >> 1) & notHFile & empty;
    frontier |= (own << 9) & notAFile & empty;
    frontier |= (own >> 7) & notAFile & empty;
    frontier |= (own << 7) & notHFile & empty;
    frontier |= (own >> 9) & notHFile & empty;

    return popcount(frontier);
}

int get_stable(board_t *board, int c) {
    const uint64_t top    = 0xff00000000000000;
    const uint64_t bottom = 0x00000000000000ff;
    const uint64_t left   = 0x0101010101010101;
    const uint64_t right  = 0x8080808080808080;

    uint64_t own, opp, pcs, vert, horiz, diag1, diag2, stable_own, stable_opp;

    if (c == BLACK) {
        own = board->b;
        opp = board->w;
    } else {
        own = board->w;
        opp = board->b;
    }

    pcs = board->b | board->w;

    vert  = nortOccl(bottom & pcs, pcs) & soutOccl(top & pcs, pcs);
    horiz = eastOccl(left & pcs, pcs) & westOccl(right & pcs, pcs);
    diag1 = noEaOccl((bottom | left) & pcs, pcs) & soWeOccl((top | right) & pcs, pcs);
    diag2 = noWeOccl((bottom | right) & pcs, pcs) & soEaOccl((top | left) & pcs, pcs);

    stable_own = (0x8100000000000081 | (vert & horiz & diag1 & diag2)) & own;
    stable_opp = (0x8100000000000081 | (vert & horiz & diag1 & diag2)) & opp;

    /* Expand the stable areas. */
    for (int ii = 0; ii < 8; ++ii) {
        stable_own |= own & (
            (nortOne(stable_own) | soutOne(stable_own) | vert) &
            (eastOne(stable_own) | westOne(stable_own) | horiz) &
            (noEaOne(stable_own) | soWeOne(stable_own) | diag1) &
            (noWeOne(stable_own) | soEaOne(stable_own) | diag2)
        );
        stable_opp |= opp & (
            (nortOne(stable_opp) | soutOne(stable_opp) | vert) &
            (eastOne(stable_opp) | westOne(stable_opp) | horiz) &
            (noEaOne(stable_opp) | soWeOne(stable_opp) | diag1) &
            (noWeOne(stable_opp) | soEaOne(stable_opp) | diag2)
        );
;
    }

    return popcount(stable_own) - popcount(stable_opp);
}




/*
 * Make-move: Makes a move for color c in position pos, and updates the board's
 * hash. Give -1 as pos for pass.  Gen is a one-hot long representing the added
 * piece. Filling from gen along opponent pieces and &-ing with rays in the
 * opposite direction from existing pieces gives only lines of opponent pieces
 * that have the new piece on one side and an existing own piece on the other
 * side.
 */
void do_move(board_t *board, int pos, int c) {
    uint64_t gen, own, pro, diff;

    /* -1 for pass. */
    if (pos == -1) {
        return;
    }

    gen = 1L << pos;

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
    board->hash ^= hash_vals[c][pos];
}

void add_piece(board_t *board, int pos, int c) {
    if (c == BLACK) {
        board->b |= (1L << pos);
    } else {
        board->w |= (1L << pos);
    }

    board->hash ^= hash_vals[c][pos];
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
                fprintf(stderr, "W ");
            } else if (b) {
                fprintf(stderr, "B ");
            } else {
                fprintf(stderr, "_ ");
            }
        }
        fprintf(stderr, "\n");
    }
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
