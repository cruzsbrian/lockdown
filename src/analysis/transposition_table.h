#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include "../board/board.h"

#define N_SLOTS 0xffff

/* Linked list node. */
typedef struct _node {
    board_t *key;
    board_t value;
    struct _node *next;
} node_t;


/* Hash table. */
typedef struct {
    node_t **slot;
} hash_table_t;


int hash(board_t *key);


/* Linked list utilities. */
node_t *create_node(board_t *key, int value);
void free_list(node_t *list);

/* Hash table utilities. */
hash_table_t *ht_create(void);
void ht_free(hash_table_t *ht);

char ht_get(hash_table_t *ht, board_t *key);
int ht_contains(hash_table_t *ht, board_t *key);
void ht_put(hash_table_t *ht, board_t *key, char value);

#endif
