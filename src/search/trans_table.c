#include "trans_table.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


uint32_t hash(board_t key);
int is_equal(board_t key1, board_t key2) {
    return key1.w == key2.w && key1.b == key2.b;
}


uint32_t hash(board_t key) {
    return key.hash;
}


node_t *init_trans_table() {
    int ii;

    node_t *tt = (node_t *)malloc(NSLOTS * sizeof(node_t));

    for (ii = 0; ii < NSLOTS; ++ii) {
        board_t key = { 0L, 0L, 0L };
        tt[ii].key = key;
    }

    return tt;
}

void free_trans_table(node_t *tt) {
    free(tt);
}


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

void set_score(node_t *tt, board_t key, char color, int16_t score, char depth, char type) {
    uint32_t hash_val;
    node_t *slot;
    node_t new = { key, color, score, depth, type };

    hash_val = hash(key);
    slot = tt + hash_val;

    *slot = new;
}
