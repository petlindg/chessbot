#include "types.h"
#include "bot.h"
#include "interface.h"
#include "check.h"
#include "board.h"
#include "move.h"
#include <stdio.h>
#include <stdlib.h>

void play_negaMax() {
    Piece board[8][8];  //matrix to keep track of current board state
    Move* moves;        //array to keep track of legal moves
    int size=1;         //keeps track of amount of legal moves to iterate over var moves
    int moveCount=0;    //keeps track of total amount of half-moves
    Color playColor;    //current players color
    Color oppColor;     //current opponents color
    Move move;          //current move

    int depth = 4;      //defines search depth for the negaMax algorithm where depth is number of half-moves
                        //depth 4 takes ~10s to evaluate on my machine, depth 5 takes ~3 minutes

    initBoard(board);   //initializes the board to the default chess board state
    printBoard(board);

    //main game loop
    while(size) {
        moves = malloc(10*sizeof(Move));
        //decides current player
        if(moveCount%2) {
            playColor = BLACK;
            oppColor = WHITE;
            //get all legal moves
            size = getMoves(board, playColor, &moves);
            //get best move negaMax algorithm finds
            move = negaMax(board, playColor, oppColor, depth);  
        } else {
            playColor = WHITE;
            oppColor = BLACK;
            //get all legal moves
            size = getMoves(board, playColor, &moves);
            if(!size) break;
            printMoves(moves, size);
            //prompt for move, only returns if legal move is input
            move = promptMove(moves, size);
        }

        //stop game if moveCount is excessive or if there is no legal moves available
        if(moveCount>=500||!size) {
            free(moves);
            break;
        }

        //applies the var move to the board, updating the state
        movePiece(board, move);

        free(moves);
        moveCount++;
        printBoard(board);
        printMove(move);
    }

    //print post-game stats
    //print winner
    if(!size) {
        if(isCheck(board, playColor)) {
            if(oppColor == BLACK) {
                printf("Black won.\n");
            } else {
                printf("White won.\n");
            }
        } else {
            printf("Draw.\n");
        }
    }
    //print number of half moves
    printf("Amount of half-moves: %d\n", moveCount);
    return;
}

void selfPlay_negaMax() {
    Piece board[8][8];  //matrix to keep track of current board state
    Move* moves;        //array to keep track of legal moves
    int size=1;         //keeps track of amount of legal moves to iterate over var moves
    int moveCount=0;    //keeps track of total amount of half-moves
    Color playColor;    //current players color
    Color oppColor;     //current opponents color
    Move move;          //current move

    int depth = 4;      //defines search depth for the negaMax algorithm where depth is number of half-moves
                        //depth 4 takes ~10s to evaluate on my machine, depth 5 takes ~3 minutes

    initBoard(board);   //initializes the board to the default chess board state
    printBoard(board);

    //main game loop
    while(size) {
        moves = malloc(10*sizeof(Move));
        //decides current player
        if(moveCount%2) {
            playColor = BLACK;
            oppColor = WHITE;
        } else {
            playColor = WHITE;
            oppColor = BLACK;
        }
        //get all legal moves
        size = getMoves(board, playColor, &moves);
        move = negaMax(board, playColor, oppColor, depth);  

        //stop game if moveCount is excessive or if there is no legal moves available
        if(moveCount>=500||!size) {
            free(moves);
            break;
        }

        //applies the var move to the board, updating the state
        movePiece(board, move);

        free(moves);
        moveCount++;
        printBoard(board);
        printMove(move);
    }

    //print post-game stats
    //print winner
    if(!size) {
        if(isCheck(board, playColor)) {
            if(oppColor == BLACK) {
                printf("Black won.\n");
            } else {
                printf("White won.\n");
            }
        } else {
            printf("Draw.\n");
        }
    }
    //print number of half moves
    printf("Amount of half-moves: %d\n", moveCount);
    return;
}