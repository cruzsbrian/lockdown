#ifndef TRANS_TABLE_H
#define TRANS_TABLE_H

#include <stdint.h>
#include "../board/board.h"

#define NSLOTS 65536


/* Linked list struct. */
typedef struct _node {
    board_t key;
    char turn;
    float score;
    char depth;
    struct _node *next;
} node_t;

/* Hash table struct. */
typedef struct {
    node_t **slot;
} trans_table_t;


trans_table_t *init_trans_table(void);
void free_trans_table(trans_table_t *tt);

int lookup_score(trans_table_t *tt, board_t key, char turn, float *score, char *depth);
void set_score(trans_table_t *tt, board_t key, char turn, float score, char depth);

#endif
