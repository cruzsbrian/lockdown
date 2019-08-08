#include <stdio.h>
#include <stdlib.h>

#include "search/search.h"


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
    board->hash = 0L;
    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);

    search_init();
}

/**
 * Player free: frees the board allocated for the player.
 */
void player_free(void) {
    search_free();
    free(board);
}


/**
 * Next move: takes the opponent's move and the time left and returns the bot's
 * move in response.
 */
int next_move(int opp_move, int ms_left) {
    int bot_move;

    /* Apply opponent's move. */
    do_move(board, opp_move, !bot_color);

    /* Get bot's move. */
    bot_move = search(board, bot_color, move_num);

    /* Apply bot's move. */
    do_move(board, bot_move, bot_color);

    /* Increment move_num for opponent move and bot move. */
    move_num += 2;

    return bot_move;
}
