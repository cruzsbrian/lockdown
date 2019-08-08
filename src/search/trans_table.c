#include "trans_table.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


uint16_t hash(board_t key);
int is_equal(board_t key1, board_t key2, char turn1, char turn2) {
    return key1.w == key2.w && key1.b == key2.b && turn1 == turn2;
}

node_t *create_node(board_t key, char turn, float score, char depth);
void free_list(node_t *list);


uint16_t hash(board_t key) {
    return key.hash;
}


node_t *create_node(board_t key, char turn, float score, char depth) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (new_node == NULL) {
        fprintf(stderr, "Error: ran out of memory for trans table.");
        exit(1);
    }

    new_node->key   = key;
    new_node->turn  = turn;
    new_node->score = score;
    new_node->depth = depth;
    new_node->next  = NULL;

    return new_node;
}

void free_list(node_t *list) {
    node_t *tmp;

    while (list) {
        tmp = list->next;
        free(list);
        list = tmp;
    }
}


trans_table_t *init_trans_table() {
    int ii;

    trans_table_t *tt = (trans_table_t *)malloc(sizeof(trans_table_t));
    tt->slot = (node_t **)malloc(NSLOTS * sizeof(node_t *));

    for (ii = 0; ii < NSLOTS; ++ii) {
        tt->slot[ii] = NULL;
    }

    return tt;
}

void free_trans_table(trans_table_t *tt) {
    int ii;

    for (ii = 0; ii < NSLOTS; ++ii) {
        free_list(tt->slot[ii]);
    }

    free(tt->slot);

    free(tt);
}


int lookup_score(trans_table_t *tt, board_t key, char turn, float *score, char *depth) {
    uint16_t hash_val;
    node_t *list;

    hash_val = hash(key);
    list = tt->slot[hash_val];

    while(list && !is_equal(key, list->key, turn, list->turn)) {
        list = list->next;
    }

    if (list) {
        *score = list->score;
        *depth = list->depth;
        return 1;
    }

    return 0;
}

void set_score(trans_table_t *tt, board_t key, char turn, float score, char depth) {
    uint16_t hash_val;
    node_t *list, *new_node;

    hash_val = hash(key);
    list = tt->slot[hash_val];

    while(list && !is_equal(key, list->key, turn, list->turn)) {
        list = list->next;
    }

    if (list) {
        list->score = score;
    } else {
        new_node = create_node(key, turn, score, depth);
        new_node->next = tt->slot[hash_val];
        tt->slot[hash_val] = new_node;
    }
}

