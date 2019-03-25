#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board/board.h"
#include "search/search.h"
#include "search/negamax.h"
#include "eval/table_eval.h"

int get_human_move(board_t *board, int c);
int get_bot_move(board_t *board, int c, int move_num);

int main(int argc, char *argv[]) {
    board_t *board;
    int turn, bot_color, move, move_num;

    /* Get bot color from args. */
    if (argc != 2) {
        fprintf(stderr, "Missing bot color arg.\n");
        exit(1);
    }

    bot_color = strcmp(argv[1], "0");
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
            move = get_bot_move(board, turn, move_num);
            printf("Bot move: %d\n", move);
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


int get_bot_move(board_t *board, int c, int move_num) {
    if (move_num > 60 - 12) {
        return negamax(board, c, 13).pos;
    } else {
        return negamax(board, c, 7).pos;
    }
}

int get_human_move(board_t *board, int c) {
    int move;

    printf("Your move: ");
    if (!get_moves(board, c)) {
        printf("pass\n");
        return -1;
    }

    scanf("%d", &move);
    while (!((1L << move) & get_moves(board, c))) {
        printf("Illegal move\nYour move: ");
        scanf("%d", &move);
    }

    return move;
}
