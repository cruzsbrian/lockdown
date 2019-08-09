#include "trans_table.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


uint32_t hash(board_t key);
int is_equal(board_t key1, board_t key2, char turn1, char turn2) {
    return key1.w == key2.w && key1.b == key2.b && turn1 == turn2;
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
        tt[ii].turn = 0;
        tt[ii].score = 0.;
        tt[ii].depth = 0;
    }

    return tt;
}

void free_trans_table(node_t *tt) {
    free(tt);
}


int lookup_score(node_t *tt, board_t key, char turn,
                 int16_t *score, char *depth, char *type) {
    uint32_t hash_val;
    node_t slot;

    hash_val = hash(key);

    /* fprintf(stderr, "\tLooking up trans table @ hash %d\n\t Board %lx %lx\n", hash_val, key.b, key.w); */

    slot = tt[hash_val];

    if (is_equal(slot.key, key, slot.turn, turn)) {
        /* fprintf(stderr, "\tFound, score %.2f for color %d\n", slot.score, slot.turn); */
        *score = slot.score;
        *depth = slot.depth;
        return 1;
    }

    return 0;
}

void set_score(node_t *tt, board_t key, char turn, int16_t score, char depth) {
    uint32_t hash_val;

    hash_val = hash(key);

    /* fprintf(stderr, "\tSetting trans table @ hash %d\n\t Board %lx %lx\n", hash_val, key.b, key.w); */

    tt[hash_val].key = key;
    tt[hash_val].turn = turn;
    tt[hash_val].score = score;
    tt[hash_val].depth = depth;
}
