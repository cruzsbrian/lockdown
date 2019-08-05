#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include "board/board.h"
#include "search/search.h"
#include "search/alphabeta.h"
#include "eval/table_eval.h"

int get_human_move(board_t *board, int c);
move_score_t get_bot_move(board_t *board, int c, int move_num);

/**
 * Main function. Runs a game against a human's input.
 * Arg specifies the bot's color, can be "0"/"black" or "1"/"white".
 */
int main(int argc, char *argv[]) {
    board_t *board;
    int turn, bot_color, move, move_num;
    move_score_t bot_move;

    /* Get bot color from args. */
    /* Check that bot color arg was given. */
    if (argc != 2) {
        fprintf(stderr, "Missing bot color arg.\n");
        exit(1);
    }

    /* Parse bot color arg (number or string). Exit with error if not valid. */
    if (strcmp(argv[1],"0") == 0 || strcmp(argv[1],"black") == 0) {
        bot_color = BLACK;
    }
    else if (strcmp(argv[1],"1") == 0 || strcmp(argv[1],"white") == 0 ) {
        bot_color = WHITE;
    }
    else {
        fprintf(stderr, "Invalid bot color argument.\n");
        exit(1);
    }

    printf("Bot color: %s\n", bot_color ? "white" : "black");


    /* Set up the board, with the four starting pieces. */
    board = create_board();
    board->b = 0L;
    board->w = 0L;
    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);


    /* Run the game. */
    print_board(board);

    turn = BLACK;
    move_num = 0;

    /* Loop until neither player can move. */
    while (get_moves(board, BLACK) || get_moves(board, WHITE)) {
        if (turn == bot_color) {
            int movecol;
            char moverow;

            /* Get move from bot, parse into row and column format */
            bot_move = get_bot_move(board, turn, move_num);
            move = bot_move.pos;
            movecol = (move % 8) + 1;
            moverow = 'a' + (move - movecol + 1) / 8;

            /* Print move with score. */
            printf("\nBot move: %c%d (score %.2f)\n",
                    moverow, movecol, bot_move.score);
        } else {
            move = get_human_move(board, turn);
        }

        /* Apply the move and print the board. */
        do_move(board, move, turn);
        print_board(board);

        /* Advance by one turn, switch sides. */
        move_num++;
        turn = !turn;
    }


    /* Game over: print winner/draw. */
    printf("Game over: ");
    if (popcount(board->b) > popcount(board->w)) {
        printf("black wins.\n");
    } else if (popcount(board->b) < popcount(board->w)) {
        printf("white wins.\n");
    } else {
        printf("draw.\n");
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

    /* Run search. If <=15 moves out, solve to game end. */
    if (move_num >= 60 - 15) {
        printf("Running end-game solver.\n");
        result = ab_search(board, c, 60, &n_nodes);
    } else {
        printf("Running alphabeta search.\n");
        result = ab_search(board, c, 9, &n_nodes);
    }

    end = clock();

    /* Convert search time to seconds, find nodes per second, print. */
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    nps = (float)n_nodes / seconds;
    printf("%ld nodes in %.2fs @ %.0f node/s\n", n_nodes, seconds, nps);

    /* If search reached game end and had a win state, guaranteed win. */
    if (result.end && result.score > 0) {
        printf("Guaranteed win\n");
    }

    return result;
}

/**
 * Get move from human input. Check that move is legal and reprompt if needed.
 * If human has no legal moves, automatically pass (print this as their move and
 * return -1).
 */
int get_human_move(board_t *board, int c) {
    int move;
    char moverow;

    /* Get set of legal moves for input checking. */
    uint64_t legal_moves = get_moves(board, c);

    /* 
     * Prompt for human move. If there are no legal moves, print pass and
     * immediately return -1.
     */
    printf("Opponent move: ");
    if (!legal_moves) {
        printf("pass\n");
        return -1;
    }

    /* 
     * Scan for human move. Include a space in the format string so the \n from
     * the last input doesn't get counted.
     */
    scanf(" %c%d", &moverow, &move);

    /* Convert row-column format to 0-63 move format. */
    move--;
    move += 8 * (moverow - 'a');

    /* Check that the move is legal. If not, reprompt recursively. */
    if (!((1L << move) & legal_moves)) {
        printf("Illegal move \"%d\"\n", move);
        return get_human_move(board, c);
    }

    return move;
}
