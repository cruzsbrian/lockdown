#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include "board/board.h"
#include "search/search.h"
#include "search/alphabeta.h"

move_score_t get_bot_move(board_t *board, int c, int move_num);
int get_opp_move(void);

void print_move(move_score_t move);

/**
 * Main function. Runs a game against a human's input.
 * Arg specifies the bot's color, can be "0"/"black" or "1"/"white".
 */
int main(int argc, char *argv[]) {
    board_t *board;
    int bot_color, move_num;
    int opp_move, ms_left;
    move_score_t bot_move;

    /* Get bot color from args. */
    /* Check that bot color arg was given. */
    if (argc != 2) {
        fprintf(stderr, "Missing bot color arg.\n");
        exit(1);
    }

    /* Parse bot color arg (number or string). Exit with error if not valid. */
    if (strcmp(argv[1],"Black") == 0) {
        bot_color = BLACK;
    } else {
        bot_color = WHITE;
    }

    /* Set up the board, with the four starting pieces. */
    board = create_board();
    board->b = 0L;
    board->w = 0L;
    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    move_num = 0;

    printf("Init done \n");
    fflush(stdout);

    while (1) {
        opp_move = get_opp_move();

        do_move(board, opp_move, !bot_color);

        bot_move = get_bot_move(board, bot_color, move_num);
        do_move(board, bot_move.pos, bot_color);

        print_move(bot_move);

        move_num += 2;
    }

    free(board);

    return 0;
}


/**
 * Get bot move. Runs the bot to get its move for a board state, color, and move
 * number. -1 indicates a pass.
 */
move_score_t get_bot_move(board_t *board, int c, int move_num) {
    move_score_t result;
    long n_nodes = 0L;
    clock_t start, end;
    float seconds, nps;

    start = clock();

    /* Run search. If <=20 moves out, solve to game end. */
    if (move_num >= 60 - 20) {
        fprintf(stderr, "Running end-game solver.\n");
        result = endgame_search(board, c, &n_nodes);
    } else {
        fprintf(stderr, "Running alphabeta search.\n");
        result = ab_search(board, c, 9, &n_nodes);
    }

    end = clock();

    /* Convert search time to seconds, find nodes per second, print. */
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    nps = (float)n_nodes / seconds;
    fprintf(stderr, "%ld nodes in %.2fs @ %.0f node/s\n",
            n_nodes, seconds, nps);

    return result;
}

int get_opp_move(void) {
    int x, y, ms_left;

    scanf("%d %d %d", &x, &y, &ms_left);
    fprintf(stderr, "%d %d \n", x, y);

    if (x == -1 && y == -1) return -1;

    return y * 8 + x;
}

void print_move(move_score_t move) {
    int x, y;

    if (move.pos == -1) {
        x = -1;
        y = -1;
    } else {
        x = move.pos % 8;
        y = move.pos / 8;
    }

    printf("%d %d\n", x, y);
    fflush(stdout);
}
