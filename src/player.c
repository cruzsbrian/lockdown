#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>

#include "board/board.h"
#include "search/search.h"
#include "search/alphabeta.h"


board_t *board;
int bot_color, move_num;


/**
 * Player init: initializes the player's board, move count, and color with the
 * value given by c.
 */
void player_init(int c) {
    bot_color = c;
    move_num = c; /* BLACK is 0, WHITE is 1, giving starting move_num. */

    board = create_board();
    board->b = 0L;
    board->w = 0L;
    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);
}

/**
 * Player free: frees the board allocated for the player.
 */
void player_free(void) {
    free(board);
}


/**
 * Next move: takes the opponent's move and the time left and returns the bot's
 * move in response.
 */
int next_move(int opp_move, int ms_left) {
    move_score_t result;
    long n_nodes = 0L;
    clock_t start, end;
    float seconds, nps;

    /* Apply opponent's move. */
    do_move(board, opp_move, !bot_color);

    start = clock();

    /* Run search. If <=20 moves out, solve to game end. */
    if (move_num >= 60 - 20) {
        fprintf(stderr, "Running end-game solver.\n");
        result = endgame_search(board, bot_color, &n_nodes);
    } else {
        fprintf(stderr, "Running alphabeta search.\n");
        result = ab_search(board, bot_color, 9, &n_nodes);
    }

    end = clock();

    /* Apply bot's move. */
    do_move(board, result.pos, bot_color);

    /* Convert search time to seconds, find nodes per second, print. */
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    nps = (float)n_nodes / seconds;
    fprintf(stderr, "%ld nodes in %.2fs @ %.0f node/s\n",
            n_nodes, seconds, nps);

    /* Increment move_num for opponent move and bot move. */
    move_num += 2;

    return result.pos;
}
