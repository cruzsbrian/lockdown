#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "board/board.h"
#include "search/search.h"
#include "search/alphabeta.h"
#include "eval/table_eval.h"

int get_human_move(board_t *board, int c);
move_score_t get_bot_move(board_t *board, int c, int move_num);

int main(int argc, char *argv[]) {
    board_t *board;
    int turn, bot_color, move, move_num;
    move_score_t bot_move;

    /* Get bot color from args. */
    if (argc != 2) {
        fprintf(stderr, "Missing bot color arg.\n");
        exit(1);
    }

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


    board = create_board();
    board->b = 0L;
    board->w = 0L;

    add_piece(board, 27, WHITE);
    add_piece(board, 28, BLACK);
    add_piece(board, 35, BLACK);
    add_piece(board, 36, WHITE);


    print_board(board);

    turn = BLACK;
    move_num = 0;

    while (get_moves(board, BLACK) || get_moves(board, WHITE)) {
        if (turn == bot_color) {
            int movecol;
            char moverow;
            bot_move = get_bot_move(board, turn, move_num);
            move = bot_move.pos;
            movecol = (move + 1) % 8;
            if (movecol == 0) movecol += 8;
            moverow = 'a' + (move - movecol + 1) / 8;

            printf("Bot move: %c%d (%d)\n", moverow, movecol, move);
            printf("Score: %.2f\n", bot_move.score);
            if (bot_move.end && bot_move.score > 0) {
                printf("Guaranteed win.\n");
            }
        } else {
            move = get_human_move(board, turn);
        }

        do_move(board, move, turn);
        print_board(board);

        move_num++;
        turn = !turn;
    }


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


move_score_t get_bot_move(board_t *board, int c, int move_num) {
    if (move_num > 60 - 22) {
        return alphabeta(board, c, -FLT_MAX, FLT_MAX, 24);
    } else {
        return alphabeta(board, c, -FLT_MAX, FLT_MAX, 10);
    }
}

int get_human_move(board_t *board, int c) {
    int move;
    char moverow;

    uint64_t legal_moves = get_moves(board, c);

    printf("Opponent move: ");
    if (!legal_moves) {
        printf("pass\n");
        return -1;
    }

    /* Include a space so that the \n from the last one doesn't get counted. */
    scanf(" %c%d", &moverow, &move);
    /*printf("%c, %d\n", moverow, move);*/

    move--;
    move += 8 * (moverow - 'a');


    if (!((1L << move) & legal_moves)) {
        printf("Illegal move \"%d\"\n", move);
        return get_human_move(board, c);
    }

    return move;
}
