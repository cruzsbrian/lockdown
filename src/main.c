#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "player.h"


int read_move(int *move, int *ms_left);
void print_move(int move);


/**
 * Main function. Runs a game against a human's input.
 * Arg specifies the bot's color, can be "0"/"black" or "1"/"white".
 */
int main(int argc, char *argv[]) {
    int bot_color, opp_move, bot_move, ms_left;
    clock_t start;

    /* Get bot color from args. */
    /* Check that bot color arg was given. */
    if (argc != 2) {
        fprintf(stderr, "Missing bot color arg.\n");
        exit(1);
    }

    /* Parse bot color arg (number or string). Exit with error if not valid. */
    if (strcmp(argv[1],"Black") == 0) {
        bot_color = 0;
    } else {
        bot_color = 1;
    }

    /* Initialize player and tell the server we're ready. */
    player_init(bot_color);
    printf("Init done \n");
    fflush(stdout);

    start = clock();

    /* Game loop. */
    while (read_move(&opp_move, &ms_left)) {
        if (ms_left == -1) {
            ms_left = 240000 - (clock() - start) / (CLOCKS_PER_SEC / 1000);
        }

        /* Print blank lines before/after our debug messages. */
        fprintf(stderr, "\n");
        bot_move = next_move(opp_move, ms_left);
        fprintf(stderr, "\n");

        print_move(bot_move);
    }

    player_free();

    return 0;
}


/**
 * Read move: reads a move and time left in "x y ms_left" format and stores the
 * move as a 0-63 index in *move and the time in *ms_left.
 */
int read_move(int *move, int *ms_left) {
    int x, y, code;

    code = scanf("%d %d %d", &x, &y, ms_left) != EOF;

    if (x == -1 && y == -1) {
        *move = -1;
    } else {
        *move = y * 8 + x;
    }
    
    return code;
}

/**
 * Print move: takes a move as a 0-63 index and prints it in "x y" format to
 * stdout.
 */
void print_move(int move) {
    int x, y;

    if (move == -1) {
        x = -1;
        y = -1;
    } else {
        x = move % 8;
        y = move / 8;
    }

    printf("%d %d\n", x, y);
    fflush(stdout);
}
