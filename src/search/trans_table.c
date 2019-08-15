#include "trans_table.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


uint32_t hash(board_t key);
int is_equal(board_t key1, board_t key2);


/* Get the hash of a board. */
uint32_t hash(board_t key) {
    return key.hash;
}

/* Compare two boards. Returns 1 if equal, else 0. */
int is_equal(board_t key1, board_t key2) {
    return key1.w == key2.w && key1.b == key2.b;
}


/**
 * Init transposition table:
 * Allocates memory for the transposition table and fills it with blank keys
 * that no actual board will match.
 */
node_t *init_trans_table() {
    int ii;

    node_t *tt = (node_t *)malloc(NSLOTS * sizeof(node_t));

    for (ii = 0; ii < NSLOTS; ++ii) {
        board_t key = { 0L, 0L, 0L };
        tt[ii].key = key;
    }

    return tt;
}

/* Free transposition table memory. */
void free_trans_table(node_t *tt) {
    free(tt);
}


/**
 * Lookup score for node:
 * Gets the slot corresponding to a given key, checks that the keys match, and
 * if they do returns it. If not, depth is marked 0 before returning to signal
 * that the score is invalid.
 */
node_t lookup_score(node_t *tt, board_t key) {
    uint32_t hash_val;
    node_t *slot;
    node_t ret;

    hash_val = hash(key);
    slot = tt + hash_val;

    if (is_equal(slot->key, key)) {
        return *slot;
    }

    ret = *slot;
    ret.depth = 0;
    return ret;
}

/**
 * Set score for node:
 * Sets the value of the table entry for a given board key with color, score,
 * depth, and node type provided.
 */
void set_score(node_t *tt, board_t key, char color, int16_t score, char depth, char type) {
    uint32_t hash_val;
    node_t *slot;
    node_t new = { key, color, score, depth, type };

    hash_val = hash(key);
    slot = tt + hash_val;

    *slot = new;
}
