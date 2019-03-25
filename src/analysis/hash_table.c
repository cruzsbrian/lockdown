#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include "../board/board.h"


int hash(board_t *key) {
    return (key->b + key->w) % N_SLOTS;
}


/* Create a single node. */
node_t *create_node(board_t *key, int value) {
    /* Allocate memory for the node. */
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        fprintf(stderr, "Error: unable to allocate memory for list node.");
        exit(1);
    }

    /* Initialize the node. */
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;

    return new_node;
}

void free_list(node_t *list) {
    node_t *tmp;

    while (list) {
        tmp = list->next;

        free(list->key);
        free(list);

        list = tmp;
    }
}


hash_table_t *ht_create(void) {
    int ii;

    /* Allocate memory for the hash table. */
    hash_table_t *new_ht = (hash_table_t *)malloc(sizeof(hash_table_t));
    if (new_ht == NULL) {
        fprintf(stderr, "Error: unable to allocated memory for hash table.");
        exit(1);
    }

    /* Initialize all the slots as empty lists. */
    for (ii = 0; ii < N_SLOTS; ++ii) {
        new_ht->slot[ii] = NULL;
    }

    return new_ht;
}

void ht_free(hash_table_t *ht) {
    int ii;

    /* Free each list. */
    for (ii = 0; ii < N_SLOTS; ++ii) {
        free_list(ht->slot[ii]);
    }

    /* Free the array of pointers to lists. */
    free(ht->slot);

    /* Free the table itself. */
    free(ht);
}


/*
 * Look for a key in the hash table. Return -1 if not found.
 * If it is found return the associated value.
 */
char ht_get(hash_table_t *ht, board_t *key) {
    int hash_val;
    node_t *list;

    hash_val = hash(key);
    list = ht->slot[hash_val];

    /*
     * Search the list until we hit the right key or the end of the list.
     * Note that if list is NULL, the statement short cicruits, so it won't
     * try to get the key for the string comparison.
     */
    while (list && board_compare(list->key, key)) {
        list = list->next;
    }

    if (list) { /* We found the key before the end of the list. */
        return list->value;
    } else {    /* We hit the end of the list without finding the key. */
        return -1;
    }
}

/*
 * Returns 1 if the table contains the value, 0 otherwise.
 */
int ht_contains(hash_table_t *ht, board_t *key) {
    return ht_get(ht, key) != -1;
}

/*
 * Set the value stored at a key. If the key is not in the table,
 * create a new node and set the value to 'value'. Note that this
 * function alters the hash table that was passed to it.
 */
void ht_put(hash_table_t *ht, board_t *key, char value) {
    int hash_val;
    node_t *list, *new_node;

    hash_val = hash(key);
    list = ht->slot[hash_val];

    /* Search through the existing list for the key.
     * (see the comment in get_value() for more explanation)
     */
    while (list && board_compare(list->key, key)) {
        list = list->next;
    }

    if (list) { /* We found the key before the end of the list. */
        /* Update the value. */
        list->value = value;
        free(key);
    } else {    /* We hit the end of the list without finding the key. */
        /* We will add a new node to the start of this slot. */
        new_node = create_node(key, value);

        /* Have the new node come before whatever's already in the slot. */
        new_node->next = ht->slot[hash_val];

        /* Have the slot point to the new node. */
        ht->slot[hash_val] = new_node;
    }
}
