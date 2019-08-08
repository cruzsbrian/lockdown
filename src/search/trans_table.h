#ifndef TRANS_TABLE_H
#define TRANS_TABLE_H

#include <stdint.h>
#include "../board/board.h"

#define NSLOTS 0x1000000    /* 24 bit hashes */


/* Node for storing game position and score. */
typedef struct {
    board_t key;
    char turn : 1;
    float score;
    char depth;
} node_t;


node_t *init_trans_table(void);
void free_trans_table(node_t *tt);

int lookup_score(node_t *tt, board_t key, char turn, float *score, char *depth);
void set_score(node_t *tt, board_t key, char turn, float score, char depth);

#endif
