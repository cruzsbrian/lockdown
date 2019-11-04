#include "opening.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t book_size;

opening_t *init_opening_book() {
    FILE *fptr;
    char *line = NULL;
    size_t len = 0;
    int ii;

    fptr = fopen("openings.txt", "r");

    /* Try to open file and read first line. If can't, quit. */
    if (!fptr || !getline(&line, &len, fptr)) {
        fprintf(stderr, "Couldn't read opening book.\n");
        exit(1);
    }

    /* First line tells how many lines the file should have. */
    book_size = atoi(line);

    fprintf(stderr, "Reading %ld lines into opening book.\n", book_size);

    /* Allocated memory for number of positions in file. */
    opening_t *ob = (opening_t *)malloc(book_size * sizeof(opening_t));

    for (ii = 0; ii < book_size; ++ii) {
        char *tmpstr;

        /* Read each line, checking for success/fail. */
        if (!getline(&line, &len, fptr)) {
            fprintf(stderr, "Could not read line %d of opening book.\n", ii);
            break;
        }

        tmpstr = (char *)malloc(len * sizeof(char));

        /* First 18 chars are the "taken" bitboard. */
        strncpy(tmpstr, line, 18);
        ob[ii].taken = strtol(tmpstr, NULL, 16);

        /* Then a space, followed by another 18 chars for black bitboard. */
        strncpy(tmpstr, line + 19, 18);
        ob[ii].black = strtol(tmpstr, NULL, 16);

        /* Lastly, the move to take. */
        strncpy(tmpstr, line + 38, len - 38);
        ob[ii].pos = atoi(tmpstr);

        free(tmpstr);
    }

    free(line);

    return ob;
}

void free_opening_book(opening_t *ob) {
    free(ob);
}

int8_t search_book(opening_t *ob, board_t *board) {
    size_t min = 0;
    size_t max = book_size - 1;

    uint64_t target_taken = board->b | board->w;
    uint64_t target_black = board->b;

    fprintf(stderr, "Searching for %lx %lx\n", target_taken, target_black);

    if (book_size == 0) {
        return -1;
    }

    while (max >= min) {
        size_t mid = (max + min) / 2;

        uint64_t taken = ob[mid].taken;
        uint64_t black = ob[mid].black;

        if (target_taken == taken && target_black == black) {
            return ob[mid].pos;
        } else if (taken < target_taken || (taken == target_taken && black < target_black)) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }

    return -1;
}
